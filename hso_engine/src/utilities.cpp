#include <hso/utilities.h>
//
#include <hso/collinear_factorization.h>//TODO: only hso::SetLhapdf is needed from this header, modify to get rid of it
#include <hso/read_data.h>
#include <hso/data_class.h>
#include <hso/theory_class.h>
#include <hso/stat_class.h>
#include <hso/read_para.h>
#include <hso/read_kin.h>
#include <hso/objects_data.h>
#include <hso/objects_stat.h>
#include <hso/FCN.h>
#include <hso/output_dir_tree.h>
//
//Minuit functions
//
#include <gsl/gsl_math.h>                  // for gsl_function
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_errno.h>//error handling
#include <gsl/gsl_matrix_double.h>         // for gsl_matrix_calloc, gsl_mat...
#include <gsl/gsl_vector_double.h>         // for gsl_vector_calloc, gsl_vec...
//
#include <filesystem>// directory_iterator
#include <string>
#include <vector>
#include <tuple>
#include <map>                             // for map, operator==, _Rb_tree_...
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <utility>

using  std::string;

#include <LHAPDF/LHAGlue.h>                // for setVerbosity, Verbosity

#include "Minuit2/MnMigrad.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/MinuitParameter.h"
#include <Minuit2/MnApplication.h>         // for MnApplication
#include <Minuit2/MnUserCovariance.h>      // for MnUserCovariance
#include <Minuit2/MnUserParameterState.h>  // for MnUserParameterState

namespace fs = std::filesystem;

namespace hso{

  std::string home_dir {""};

  std::string eigen_filename_global {""};

  std::string kinematics_filename {""};

  std::string para_filename {""};

  std::string minimum_dir("./min/");

  namespace utils{

    bool verbose=true;
    //initialize vars below to defaults, InitProgram will update their values
    double lhapdf_set_member {0.0}; //LHAPDF member, set central as initial value

    double Q0 {2.0}; //input scale (all scales in GeV)

    double rg_transf_Qmax {2.0}; //Q0=input scale; rg_transf_Qmax=a in scale transformation RGScaleTransformation

    double mu_over_Q0 {1.0}; //to tune other scales mu_over_Q0 = mu/Q0, etc.

    double sqrt_zeta_over_Q0 {1.0};

    double mu_over_Q {1.0};

    double sqrt_zeta_over_Q {1.0};
    //for data cuts in fits, InitKinematics will read from file and update
    //limits are inclusive intervals
    double qT_over_Q_min {0.0}; //rho=qT/Q

    double qT_over_Q_max {0.2};

    double Ecm_min {200}; //CM energy (init value is min for E288 experiment)

    double Ecm_max {400}; //CM energy (init value is max for E288 experiment)

    double Qlow_min {4.0}; //Q is the observed lepton-pair mass.

    double Qlow_max {20.0}; //Qlow is the lower bound of the interval
    //e.g. for the bin Q in the range [4-5] GeV, Qlow = 4.0
    //Qlow_min is the smallest of such values to include in fit
    //Qlow_max is the largest of such values to include in fit

    //global vectors for parameter interaction with Minuit
    std::vector<double> para_central_value(0);

    std::vector<bool > para_is_free (0);

    std::vector<double> para_error (0);

    std::vector<bool > para_is_limited(0);

    std::vector<double> para_lower_limit (0);

    std::vector<double> para_upper_limit (0);
    //NOTE: functions called by 'Minimize'
    int GetFreeParaNumber(ROOT::Minuit2::MnUserParameters &upar){
      //get number of free and limited parameters
      int number_of_varying_parameters=0;

      for (auto &&p : upar.Parameters()) {

        bool para_varies = !( p.IsConst()||p.IsFixed() );

        if(para_varies)

          number_of_varying_parameters++;

      }

      return number_of_varying_parameters;

    }

    bool ReadSquareMatrix(std::string filename, std::string matrix_tag, std::vector<int> &row_indices,

                          std::vector<int> &col_indices, std::vector<double> &matrix) {

      std::vector<double> (0).swap(matrix);

      std::vector<std::string> rows(0);

      std::string row_raw;

      bool found=false;

      ///read and store rows
      std::ifstream input_file(filename);

      if (input_file) {

        found=true;

        while (getline(input_file,row_raw)) {

          hso::read_data::CleanLine( row_raw," " );

          if (row_raw.compare(0,matrix_tag.size(),matrix_tag)==0 ) {

            hso::read_data::CleanLine(row_raw, matrix_tag);

            hso::read_data::Replaceall(row_raw, ":", " ");

            rows.push_back(row_raw);

          }

        }

      }

      else {

        std::cout<<"Warning: "<<__FILE__<<": "<<__LINE__<<": can't open '"

                 <<filename<<"'."<<std::endl;

      }

      if(found) {

        input_file.close();
        //input values in vectors
        for(auto row: rows){

          std::stringstream row_print(row);

          std::string entry("");

          row_print>>entry;

          row_indices.push_back(stoi(entry));

          row_print>>entry;

          col_indices .push_back(stoi(entry));

          row_print>>entry;

          matrix.push_back(stod(entry));

        }

      }

      return found;

    }

    void WriteCovariance(ROOT::Minuit2::FunctionMinimum* min){

      const fs::path* cov_dir = ContextRegistry::GetInstance()->RequestContext("fitter_output")->GetSubdir("cov");

      ROOT::Minuit2::MnUserParameters upar = min->UserParameters() ;

      //NOTE:for errors
      int number_of_free_parameters = GetFreeParaNumber( upar );

      if(number_of_free_parameters>0){

        ROOT::Minuit2::MnUserCovariance cov=min->UserCovariance();

        gsl_matrix *cho_gsl = gsl_matrix_calloc (number_of_free_parameters, number_of_free_parameters);

        for (int i=0;i<number_of_free_parameters;i++) {

          for (int j=0;j<number_of_free_parameters;j++) {

            gsl_matrix_set ( cho_gsl, i, j, cov(i,j));

          }

        }

        gsl_linalg_cholesky_decomp (cho_gsl);
        //clean matrix
        for (int i=0;i<number_of_free_parameters;i++) {

          for (int j=i+1;j<number_of_free_parameters;j++) {

            gsl_matrix_set ( cho_gsl, i, j, 0.0);

          }

        }
        /////////write covariance
        std::ofstream output_file(*cov_dir / "covariance.dat");

        output_file<<"#+++++++++++covariance+++++++++++"<<std::endl;

        for (int i=0;i<number_of_free_parameters;i++) {

          for (int j=0;j<number_of_free_parameters;j++) {

            output_file<<"#cov:"<<i<<":"<<j<<":"<<cov(i,j)<<std::endl;

          }

        }

        output_file.close();

        output_file.clear();
        ////////////////write cholesky////////////////////////
        output_file.open(*cov_dir / "cholesky.dat");

        output_file<<"#+++++++++++cholesky+++++++++++"<<std::endl;

        for (int i=0;i<number_of_free_parameters;i++) {

          for (int j=0;j<number_of_free_parameters;j++) {

            output_file<<"#cho:"<<i<<":"<<j<<":"<<gsl_matrix_get(cho_gsl,i,j)<<std::endl;

          }

        }

        output_file.close();

      }

    }

    void WriteCovarianceEigen(std::string covariance_filename) {

      const fs::path* cov_dir = ContextRegistry::GetInstance()->RequestContext("fitter_output")->GetSubdir("cov");

      std::vector<double> matrix(0);

      std::vector<int> row_indices(0);

      std::vector<int> col_indices(0);
      //read existing covariance matrix and fill in gsl covariance
      bool found_covariance=utils::ReadSquareMatrix(covariance_filename,"#cov:", row_indices, col_indices, matrix);

      if(found_covariance){

        int matrix_size = static_cast<int>(matrix.size());

        int number_of_free_parameters = static_cast<int>(sqrt(matrix_size));

        gsl_matrix * cov_gsl = gsl_matrix_calloc ( number_of_free_parameters , number_of_free_parameters );

        for (int i = 0; i < matrix_size; i++)

          gsl_matrix_set(cov_gsl, row_indices[i], col_indices[i], matrix[i] );
        ///gsl calculation of eigen values and eigen vectors of covariance matrix
        gsl_eigen_symmv_workspace *Weigen = gsl_eigen_symmv_alloc(number_of_free_parameters);

        gsl_vector * eigenvalues_gsl = gsl_vector_calloc ( number_of_free_parameters);

        gsl_matrix *eigenvectors_gsl = gsl_matrix_calloc (number_of_free_parameters, number_of_free_parameters);

        gsl_eigen_symmv(cov_gsl, eigenvalues_gsl, eigenvectors_gsl, Weigen);

        gsl_eigen_symmv_free(Weigen);
        ///write eigen values and eigen vectors (matrix form)
        std::ofstream output_file(*cov_dir / "eigen.dat");

        if(output_file){

          output_file<<"#+++++++++++eigenvals+++++++++++"<<std::endl;

          for (int i=0;i<number_of_free_parameters;i++)

            output_file<<"#eva:"<<i<<":"<<gsl_vector_get(eigenvalues_gsl,i)<<std::endl;

          output_file<<"\n\n";

          output_file<<"#+++++++++++U Transformation+++++++++++"<<std::endl;

          for (int i=0;i<number_of_free_parameters;i++)

            for (int j=0;j<number_of_free_parameters;j++)

              output_file<<"#eve:"<<i<<":"<<j<<":"<<gsl_matrix_get(eigenvectors_gsl,i,j)

                        <<std::endl;

          output_file<<"\n\n";

          output_file<<"#+++++++++++U^T Transformation+++++++++++"<<std::endl;

          for (int i=0;i<number_of_free_parameters;i++)

            for (int j=0;j<number_of_free_parameters;j++)

              output_file<<"#evet:"<<i<<":"<<j<<":"<<gsl_matrix_get(eigenvectors_gsl,j,i)

                        <<std::endl;

        }

        else{

          std::cout<<"#Warning: could not open '"<<(*cov_dir / "eigen.dat").string()<<"'. "<<std::endl;

        }

      }

    }
    //NOTE: functions called by 'Errors'
    void ReadVector(std::string filename, std::string vector_tag, std::vector<double> &output){

      std::vector<double> (0).swap(output);

      std::vector<std::string> rows(0);

      std::string row_raw;
      ///read and store rows
      std::ifstream input_file(filename);

      if(input_file){

        while( getline(input_file,row_raw) ){

          hso::read_data::CleanLine( row_raw," " );

          if (row_raw.compare(0,vector_tag.size(),vector_tag)==0) {

            hso::read_data::CleanLine(row_raw, vector_tag);

            hso::read_data::Replaceall(row_raw,":"," ");

            rows.push_back(row_raw);

          }

        }

      }

      else {

        std::cout<<"Warning: "<<__FILE__<<": "<<__LINE__<<": can't open '"<<filename<<"'."

                 <<std::endl;

      }

      input_file.close();
      //input values in vectors
      for(auto row: rows){

        std::stringstream row_print(row);

        std::string entry("");

        row_print>>entry;

        row_print>>entry;

        output.push_back(stod(entry));

      }

    }

    void TransformParameters(const std::vector<double> &eve ,const std::vector<double> &parameters,

                             std::vector<double> &transformed_parameters, const std::vector<int> &free_index) {

      int size=free_index.size();

      std::vector<double> (parameters).swap(transformed_parameters);

      for(int i=0;i<size;i++){

        transformed_parameters[free_index[i]]=0;

        for(int j=0;j<size;j++){

          int index=j+size*i;

          transformed_parameters[free_index[i]] += eve[index]*parameters[free_index[j]];

        }

      }

    }

    double DistanceToBoundary (double para_value, void *Perr){

      std::vector< int> free_index =*static_cast< std::vector< int>**>(Perr)[0];

      int free_index_i =*static_cast< int**>(Perr)[1];

      double chi2 =*static_cast< double **>(Perr)[2];

      double chi2_min =*static_cast< double **>(Perr)[3];

      double delta_chi2 =*static_cast< double **>(Perr)[4];

      std::vector<double> parameters =*static_cast< std::vector<double>**>(Perr)[5];

      std::vector<double> transformed_parameters =*static_cast< std::vector<double>**>(Perr)[6];

      std::vector<double> eve =*static_cast< std::vector<double>**>(Perr)[7];

      std::vector<double> evet =*static_cast< std::vector<double>**>(Perr)[8];

      std::vector<double> eva =*static_cast< std::vector<double>**>(Perr)[9];

      ROOT::Minuit2::FCN *theFCN = static_cast<ROOT::Minuit2::FCN**>(Perr)[10];

      int index = free_index[free_index_i];

      transformed_parameters[index]=para_value;

      TransformParameters(eve ,transformed_parameters ,parameters, free_index);

      chi2=theFCN->operator()(parameters);

      return chi2-chi2_min - delta_chi2;

    }

    double Scan(double init_shift_factor,void *Perr){

      std::vector< int> free_index =*static_cast< std::vector< int>**>(Perr)[ 0];

      int free_index_i =*static_cast< int**>(Perr)[ 1];

      double chi2 =*static_cast< double **>(Perr)[ 2];

      std::vector<double> parameters =*static_cast< std::vector<double>**>(Perr)[ 5];

      std::vector<double> transformed_parameters =*static_cast< std::vector<double>**>(Perr)[ 6];

      std::vector<double> eve =*static_cast< std::vector<double>**>(Perr)[ 7];

      std::vector<double> eva =*static_cast< std::vector<double>**>(Perr)[ 9];

      ROOT::Minuit2::FCN *theFCN = static_cast<ROOT::Minuit2::FCN**>(Perr)[10];

      int index=free_index[free_index_i];

      int status;

      int iter = 0, max_iter = 100;

      const gsl_root_fsolver_type *T;

      gsl_root_fsolver *s;

      double root = 0;

      double x_lo = 0.0;

      double x_hi = 0.0;

      if(init_shift_factor>0){

        x_lo = transformed_parameters[index];

        x_hi = x_lo + init_shift_factor*std::sqrt(eva[free_index_i]);

      }

      else if(init_shift_factor<0){

        x_hi = transformed_parameters[index];

        x_lo = x_hi + init_shift_factor*std::sqrt(eva[free_index_i]);

      }

      else {

        std::cout<<"Error: 'init_shift_factor' should be != fom zero: init_shift_factor= "

                 <<init_shift_factor<<std::endl;

      }

      gsl_function F;

      F.function = &DistanceToBoundary;

      F.params = Perr;

      T = gsl_root_fsolver_brent;

      s = gsl_root_fsolver_alloc (T);

      gsl_root_fsolver_set (s, &F, x_lo, x_hi);

      do {

        iter++;

        status = gsl_root_fsolver_iterate (s);

        root = gsl_root_fsolver_root (s);

        x_lo = gsl_root_fsolver_x_lower (s);

        x_hi = gsl_root_fsolver_x_upper (s);

        status = gsl_root_test_interval (x_lo, x_hi,0, 0.001);

      }

      while (status == GSL_CONTINUE && iter < max_iter);

      gsl_root_fsolver_free(s);
      // // get and store value of chi2 for found root
      transformed_parameters[index]=root;

      TransformParameters(eve ,transformed_parameters ,parameters, free_index);

      chi2=theFCN->operator()(parameters);//stores chi2 value in "chi2"

      *static_cast<double **>(Perr)[2]=chi2;

      static_cast<std::vector<double>**>(Perr)[ 5]->swap(parameters);

      return root;

    }
    //NOTE: functions used by 'Errors'
    void ReadEigensets(std::string eigen_filename, std::vector<std::string> &eigen_para_names,

                       std::vector<double> &eigen_sets, int &numofeigen, int &numofpara) {
      ///read eigensets file
      std::ifstream eigen_input_file(eigen_filename);

      std::vector<std::string> (0).swap(eigen_para_names);

      std::vector<double> (0).swap(eigen_sets );

      numofpara =0;

      numofeigen=0;

      if(eigen_input_file){

        std::string line;

        getline(eigen_input_file,line);

        hso::read_data::SplitLine(line," ",eigen_para_names);

        numofpara = static_cast<int>(eigen_para_names.size());

        while(getline(eigen_input_file,line)) {

          std::stringstream ss(line);

          std::string word;

          while(ss>>word)

            eigen_sets.push_back(stod(word));

          numofeigen++;

        }

      }

      eigen_input_file.close();

      if(numofeigen%2!=0){

        std::cout<<"#Error: reading eigensets from file '"<<eigen_filename

                 <<"', which contains "<<numofeigen<<" sets, which should be even."

                 <<" Only +/- version implemented. Quitting."<<std::endl;

        std::exit(0);

      }

      std::cout << "\n ------- read "<< numofeigen <<" eigen sets ---------\n\n";

      for(auto val: eigen_para_names){

        std::cout<<val<<"\t\t";

        std::cout<<"\n"<<std::endl;

      }

      for(int i=0;i<numofeigen;i++) {// i-th member

        for(int j=0;j<numofpara;j++) {//j-th parameter of i-th member

          int index=j+numofpara*i;

          std::cout<<eigen_sets[index]<<"\t\t";

        }

        std::cout<<std::endl;

      }

    }

    void GetEigenset(const std::vector<double> &eigen_sets,

                     const int &numofpara,const int &i, std::vector<double> &eigen_set_i) {

      std::vector<double>(0).swap(eigen_set_i);

      std::cout << "\n ------- read "<< i <<"-th eigen set ---------\n\n";

      for(int j=0;j<numofpara;j++) {//j-th parameter of i-th member

        int index=j+numofpara*(i-1);

        double para_value=eigen_sets[index];

        std::cout<<para_value<<"\t\t";

        eigen_set_i.push_back(para_value);

      }

      std::cout << "\n ------- done ---------\n\n";

      std::cout<<std::endl;

    }

    void FilesInDir(const std::string dir, std::vector<std::string> &filenames){

      for (const auto &entry : fs::directory_iterator(dir))

        filenames.push_back( entry.path() );

    }

    void FilesInDir(const std::string dir, const std::string substring, std::vector<std::string > &filenames){

      std::vector<std::string> new_filenames(0);

      FilesInDir(dir,filenames);

      int filenames_size = static_cast<int>(filenames.size());

      for (int i = 0; i < filenames_size; i++) {

        if(filenames[i].find(substring)!=std::string::npos)

          new_filenames.push_back(filenames[i]);

      }

      new_filenames.swap(filenames);

    }

    std::string FileInDirUnique(const std::string dir, const std::string substring){

      std::vector<std::string> filenames(0);

      FilesInDir(dir,substring,filenames);

      int filenames_size = static_cast<int>(filenames.size());

      if (filenames_size==0) {

        std::cout<<"\n#Error: directory '"<<dir<<"' does not contain a file '*"

                 <<substring<<"*'. Quitting."<<std::endl;

        std::exit(0);

      }

      else if (filenames_size > 1) {

        std::cout<<"\n#Error: file '*"<<substring<<"*' is not unique in directory '"

                 <<dir<<"'. Quitting."<<std::endl;

        std::exit(0);

      }

      return filenames[0];

    }

  }//utils

  using namespace utils;

  //NOTE: user level functions
  void InitProgram(int argc, char *argv[] , std::vector<std::string> expected_args,

                   std::vector<std::string> optional_args) {

    int num_of_expected_args=expected_args.size();

    int num_of_optional_args=optional_args.size();

    if(argc-1<num_of_expected_args){

      std::cout<<"------- passed "<< argc-1 <<" arguments but need "

               << num_of_expected_args <<": -------\n";

      for (auto name: expected_args)

        std::cout<<"\t\t"<<name<<"\n";

      if (num_of_optional_args>0) {

        std::cout<<"------- optional args "<<": -------\n";

        for( auto name: optional_args )

          std::cout<<"\t\t"<<name<<"\n";

      }

      std::cout<<"------------------ Quitting. ------------------\n";

      std::exit(0);

    }
    //-- for temp backward compatibiity
    kinematics_filename.assign(argv[1]);

    para_filename.assign(argv[2]);

    home_dir = argv[3];
    //--
    fs::path kin_file(argv[1]);

    fs::path para_file(argv[2]);

    ContextRegistry* ctx_registry = ContextRegistry::GetInstance();//Meyer singleton

    OutputDirectoryTree* output_ctx = ctx_registry->CreateContext("fitter_output", argv[3], {"cov", "input", "min", "stat", "status"});

    MakeContext(output_ctx);

    output_ctx->AddFile(kin_file, "kinematics", "input");

    output_ctx->AddFile(para_file, "parameters", "input");

  }

  void InitCollinear(){
    //version for A,B,fr grids: 0=direct calc. ) 2=lhapdf grids
    LHAPDF::setVerbosity (LHAPDF::SILENT);

    std::string lhapdf_set;

    std::vector<std::string> pdfset_names={"lhapdf_set_member"};

    void *pdfset_vals[]={&lhapdf_set_member};
    //read values for lhapdf_set and lhapdf_set_member from grid.
    const OutputDirectoryTree* output_ctx = ContextRegistry::GetInstance()->RequestContext("fitter_output");

    const std::string kin_file_name = output_ctx->GetFile("kinematics")->string();

    read_kinematics::ReadKinematics (kin_file_name, pdfset_names, pdfset_vals );

    read_kinematics::ReadString (kin_file_name,"lhapdf_set",lhapdf_set);

    hso::collinear::SetLhapdf(lhapdf_set, (int) lhapdf_set_member);

  }

  void SetRange(std::vector<hso::Data*> data_pointers , std::string var_name, double var_min, double var_max) {

    for(auto data: data_pointers)

      data->SetRange(var_name, var_min, var_max);

  }

  void InitKinematics(){// reads kinematics & implements data cuts
    ///read kin from file
    std::vector<std::string> kin_names = {
      "Q0",
      "rg_transf_Qmax",
      "mu_over_Q0",
      "sqrt_zeta_over_Q0",
      "mu_over_Q",
      "sqrt_zeta_over_Q"
    };

    void *kin_vals[]={
      &Q0,
      &rg_transf_Qmax,
      &mu_over_Q0,
      &sqrt_zeta_over_Q0,
      &mu_over_Q,
      &sqrt_zeta_over_Q
    };

    std::vector<std::string> kin_names_2={
      "qT_over_Q_min",
      "qT_over_Q_max",
      "Ecm_min",
      "Ecm_max",
      "Qlow_min",
      "Qlow_max"
    };

    void *kin_vals_2[] ={&qT_over_Q_min ,&qT_over_Q_max ,&Ecm_min ,&Ecm_max ,&Qlow_min ,&Qlow_max };

    const OutputDirectoryTree* output_ctx = ContextRegistry::GetInstance()->RequestContext("fitter_output");

    const std::string kin_file_name = output_ctx->GetFile("kinematics")->string();

    read_kinematics::ReadKinematics(kin_file_name, kin_names, kin_vals);

    read_kinematics::ReadKinematics(kin_file_name, kin_names_2, kin_vals_2);
    //set qT cuts (does not update isactive in stat)
    for(auto data: hso::e288){

      std::vector<double> Q;

      data->GetVarOther("Q",Q);

      double qT_min=qT_over_Q_min*Q[0];

      double qT_max=qT_over_Q_max*Q[0];

      std::vector<hso::Data*> data_wrapped {data};

      SetRange(data_wrapped,"qT",qT_min,qT_max);

    }

    for(auto data: hso::e605){

      std::vector<double> Q;

      data->GetVarOther("Q",Q);

      double qT_min=qT_over_Q_min*Q[0];

      double qT_max=qT_over_Q_max*Q[0];

      std::vector<hso::Data*> data_wrapped {data};

      SetRange(data_wrapped,"qT",qT_min,qT_max);

    }

    for(auto data: hso::e605){

      std::vector<double> Q;

      data->GetVarOther("Q",Q);

      double qT_min=qT_over_Q_min*Q[0];

      double qT_max=qT_over_Q_max*Q[0];

      std::vector<hso::Data*> data_wrapped {data};

      SetRange(data_wrapped,"qT",qT_min,qT_max);

    }
    ///set cuts for E and Q (each call of SetRange updates isactive in stat)
    //E288
    hso::chi2_e288.SetRange("E" , Ecm_min , Ecm_max);

    hso::chi2_e288.SetRange("Q",Qlow_min,Qlow_max);
    //E605
    hso::chi2_e605.SetRange("E" , Ecm_min , Ecm_max);

    hso::chi2_e605.SetRange("Q",Qlow_min,Qlow_max);
    //NOTE: skip upsilon resonance bins (does not update isactive in stat)
    hso::e288_200_9 .set_is_active_=false;

    hso::e288_300_9 .set_is_active_=false;

    hso::e288_400_9 .set_is_active_=false;

    hso::e288_200_10.set_is_active_=false;

    hso::e288_300_10.set_is_active_=false;

    hso::e288_400_10.set_is_active_=false;

    hso::chi2_e288.UpdateIsActive();

  }

  void SetMinuitParameters( ROOT::Minuit2::MnUserParameters & upar,

                            std::vector<std::string> parameter_names) {

    std::vector<double> (0).swap(para_central_value);

    std::vector<bool > (0).swap(para_is_free );

    std::vector<double> (0).swap(para_error );

    std::vector<bool > (0).swap(para_is_limited);

    std::vector<double> (0).swap(para_lower_limit );

    std::vector<double> (0).swap(para_upper_limit );

    for (auto name : parameter_names){

      double central_value=0;

      int is_free =0;

      double error =0;

      int is_limited=0;

      double lower_limit =0;

      double upper_limit =0;

      //get initial values from file
      read_parameters::ReadParaValues(name, para_filename, central_value,

                                      is_free, error, is_limited, lower_limit, upper_limit);

      //set minuit parameters
      upar.Add( name , central_value , error );

      if(is_free ==0)

        upar.Fix(name);

      if(is_limited!=0)

        upar.SetLimits(name,lower_limit,upper_limit);

      para_central_value.push_back(central_value);

      para_is_free .push_back(is_free );

      para_error .push_back(error );

      para_is_limited.push_back(is_limited);

      para_lower_limit .push_back(lower_limit );

      para_upper_limit .push_back(upper_limit );

    }

  }

  ROOT::Minuit2::FunctionMinimum *Minimize(ROOT::Minuit2::FCN &theFCN,

                                           ROOT::Minuit2::MnUserParameters & upar ){

    int number_of_varying_parameters = GetFreeParaNumber(upar);

    theFCN.number_of_free_parameters_ = number_of_varying_parameters;

    std::vector<std::string> parameter_names(theFCN.para_names_);
    /////set output dirs and streams for minuit output
    const OutputDirectoryTree* output_ctx = ContextRegistry::GetInstance()->RequestContext("fitter_output");

    const fs::path* min_dir = output_ctx->GetSubdir("min");

    std::ofstream minimum_output_file(*min_dir / "minimum.dat");
    ///write to log file
    std::ofstream log_file(*min_dir / "fcn.log");

    log_file<<"#1.i\t";

    int i_para_names=2;

    int para_names_size = static_cast<int>(parameter_names.size());

    for(i_para_names = 2; i_para_names < para_names_size + 2; i_para_names++)

      log_file<<i_para_names<<"."<<parameter_names[i_para_names-2]<<"\t";

    log_file<<i_para_names<<".pts ";

    i_para_names++;

    log_file<<i_para_names<<".chi2 ";

    i_para_names++;

    log_file<<i_para_names<<".chi2/pt";

    i_para_names++;

    log_file<<std::endl;

    log_file<<std::scientific<<std::setprecision(6);
    ///write to screen
    i_para_names=2;

    std::cout<<"#1.i\t";

    for(i_para_names = 2; i_para_names < para_names_size + 2; i_para_names++)

      std::cout<<i_para_names<<"."<<parameter_names[i_para_names-2]<<"\t";

    std::cout<<i_para_names<<".pts ";

    i_para_names++;

    std::cout<<i_para_names<<".chi2 ";

    i_para_names++;

    std::cout<<i_para_names<<".chi2/pt";

    i_para_names++;

    std::cout<<std::endl;
    //minimize (log file is written internally by theFCN )
    theFCN.log_file_=&log_file; //this so that 'theFCN' knows where to write info.

    ROOT::Minuit2::MnMigrad migrad(theFCN,upar);

    static ROOT::Minuit2::FunctionMinimum min = migrad(); //static to return 'min', needed in main.
    //output to min file
    minimum_output_file<<min<<std::endl;
    //close log and min files
    log_file.close();

    minimum_output_file.close();
    //write again to screen
    i_para_names=2;

    std::cout<<"#1.i\t";

    for(i_para_names = 2; i_para_names < para_names_size + 2; i_para_names++)

      std::cout<<i_para_names<<"."<<parameter_names[i_para_names-2]<<"\t";

    std::cout<<i_para_names<<".pts ";

    i_para_names++;

    std::cout<<i_para_names<<".chi2 ";

    i_para_names++;

    std::cout<<i_para_names<<".chi2/pt";

    i_para_names++;

    std::cout<<std::endl;
    //print final status for the parameters in a format readable to 'read_parameters::read_para'
    const fs::path* status_dir = output_ctx->GetSubdir("status");

    const fs::path* status_file = output_ctx->GetFile("parameters");

    std::ofstream status_output_file(*status_dir / status_file->filename());

    status_output_file<<std::left;

    status_output_file<<std::setw(20)<<"#name";

    status_output_file<<std::setw(13)<<"central_value";

    status_output_file<<std::setw( 7)<<"is_free";

    status_output_file<<std::setw(13)<<"error";

    status_output_file<<std::setw(10)<<"is_limited";

    status_output_file<<std::setw(13)<<"lower_limit";

    status_output_file<<std::setw(13)<<"upper_limit";

    status_output_file<<"\n\n";

    int central_size = static_cast<int>(para_central_value.size());

    for(int i_central = 0; i_central < central_size; i_central++){

      para_central_value[i_central]=min.UserState().Value(parameter_names[i_central]);

      para_error[i_central]=min.UserState().Error(parameter_names[i_central]);

      status_output_file<<std::left;

      status_output_file<<std::setw(20)<<parameter_names[i_central];

      status_output_file<<std::setw(13)<< para_central_value[i_central];

      status_output_file<<std::setw( 7)<< para_is_free[i_central];

      status_output_file<<std::setw(13)<< para_error[i_central];

      status_output_file<<std::setw(10)<< para_is_limited[i_central];

      status_output_file<<std::setw(13)<< para_lower_limit[i_central];

      status_output_file<<std::setw(13)<< para_upper_limit[i_central];

      status_output_file<<"\n\n";

    }

    status_output_file.close();


    // rerun fit with final fixed paramters. This is to store final values of theory,
    // chi2 pt by pt (and make sure lines correspond to final status of parameters)
    for (int i_parafix = 0; i_parafix < para_names_size; i_parafix++)

      migrad.Fix(parameter_names[i_parafix].c_str());

    hso::store_values_stat = true;

    hso::store_values_theory = true;

    ROOT::Minuit2::FunctionMinimum min2 = migrad();

    hso::store_values_stat = false;

    hso::store_values_theory = false;

    if(number_of_varying_parameters > 0){

      const fs::path* cov_dir = output_ctx->GetSubdir("cov");

      WriteCovariance(&min);//jogh

      WriteCovarianceEigen(*cov_dir / "covariance.dat");


    }

    return &min;

  }

  void Write(hso::Stat *stat_in,std::string subdir){

    std::vector<std::string> list_of_filenames(0);

    std::vector<bool > write_file(0);

    const fs::path* stat_base_dir = ContextRegistry::GetInstance()->RequestContext("fitter_output")->GetSubdir("stat");
    //get all names for files
    for (auto set: *(stat_in->data_) ) {

      if(set->set_is_active_){

        if(! fs::exists(*stat_base_dir / set->dir_name_)){

          fs::create_directory(*stat_base_dir / set->dir_name_);

        }

        fs::path file_path = fs::path(*stat_base_dir / set->dir_name_ / set->file_name_).replace_extension(".stat");

        list_of_filenames.push_back(file_path.string());

        write_file.push_back(true);

      }

      else {

        list_of_filenames .push_back("");

        write_file.push_back(false);

      }

    }
    //write all files
    int filenames_size = static_cast<int>(list_of_filenames.size());

    for (int i = 0; i < filenames_size; i++)

      if (write_file[i]){

        stat_in->Write(list_of_filenames[i],i);

      }


  }

  void Write(std::vector<hso::Stat*> stat_pointers,std::string subdir){

    for(auto stat: stat_pointers)

      Write(stat,subdir);

  }

  //finds eigen sets using covariance matrix stored by minuit
  void Errors(ROOT::Minuit2::FCN &theFCN, ROOT::Minuit2::FunctionMinimum &min){

    std::vector<double> eigen_vectors(0);

    ROOT::Minuit2::MnUserParameters upar=min.UserParameters();

    int number_of_free_parameters=GetFreeParaNumber(upar);

    double chi2=0;

    double delta_chi2 = theFCN.Up();

    double chi2_min= min.Fval();

    std::string covariance_filename (home_dir + "/cov/covariance2.dat");

    std::string eigen_filename(home_dir + "/cov/eigen.dat" );
    //vectors
    std::vector< int> row_indices(0);

    std::vector< int> col_indices(0);

    std::vector< int> free_index(0);

    std::vector<double> parameters(0);

    std::vector<double> transformed_parameters(0);

    std::vector<double> eva;

    ReadVector(eigen_filename, "#eva:", eva);
    //matrices
    std::vector<double> cov;

    ReadSquareMatrix(covariance_filename, "#cov:", row_indices, col_indices, cov);

    std::vector<double> eve;

    ReadSquareMatrix(eigen_filename, "#eve:", row_indices, col_indices, eve);

    std::vector<double> evet;

    ReadSquareMatrix(eigen_filename, "#evet:", row_indices, col_indices, evet);

    int central_size = static_cast<int>(para_central_value.size());

    std::vector<bool> parameter_is_active(0);

    for ( int i=0;i<central_size; i++ )

      parameter_is_active.push_back( para_is_free[i]!=0 );

    int label=0;

    std::vector<std::string> transformed_parameter_names(theFCN.para_names_);

    for(int i = 0; i < central_size; i++) {

      if(parameter_is_active[i]) {

        std::stringstream ss;

        ss<<"Tp_"<<label;

        label++;

        ss>>transformed_parameter_names[i];

        free_index.push_back(i);

      }

      parameters.push_back(para_central_value[i]);

    }

    TransformParameters(evet,parameters,transformed_parameters,free_index);

    std::ofstream scan_output_file(home_dir + "/cov/scan.dat");

    theFCN.log_file_=&scan_output_file;

    int free_index_i=0;

    std::vector<double> parameters_cp;

    std::vector<double> transformed_parameters_cp;

    void *Perr[]={
      &free_index,
      &free_index_i,
      &chi2,
      &chi2_min,
      &delta_chi2,
      &parameters_cp,
      &transformed_parameters_cp,
      &eve,
      &evet,
      &eva,
      &theFCN
    };

    for (free_index_i=0;free_index_i<number_of_free_parameters;free_index_i++) {

      std::vector<double>(parameters).swap(parameters_cp);

      std::vector<double>(transformed_parameters).swap(transformed_parameters_cp);

      double init_shift_factor_plus = 10;

      double init_shift_factor_minus = -10;

      Scan(init_shift_factor_plus , Perr);

      scan_output_file<<"---------------------chi2="<<chi2<<"------------------------"<<std::endl;

      for(auto val: parameters_cp)

        scan_output_file<<val<<"\t";

      scan_output_file<<std::endl;

      for(auto val: parameters_cp)

        eigen_vectors.push_back(val);

      scan_output_file<<"------------------------------------------------------------"<<std::endl;

      Scan(init_shift_factor_minus, Perr);

      scan_output_file<<"---------------------chi2="<<chi2<<"------------------------"<<std::endl;

      for(auto val: parameters_cp)

        scan_output_file<<val<<"\t";

      scan_output_file<<std::endl;

      for(auto val: parameters_cp)

        eigen_vectors.push_back(val);

      scan_output_file<<"------------------------------------------------------------"<<std::endl;

    }

    std::ofstream output_file(home_dir + "/min/eigensets.dat");

    std::cout<<"\n\n";

    for (auto name: theFCN.para_names_ ) {

      std::cout <<name<<"\t";

      output_file<<name<<"\t";

    }

    std::cout <<std::endl;

    output_file<<std::endl;

    for(int i=0;i<2*number_of_free_parameters;i++){

      int para_size = static_cast<int>(parameters.size());

      for(int j=0;j<para_size;j++){

        std::cout <<eigen_vectors[j+i*para_size]<<"\t";

        output_file<<eigen_vectors[j+i*para_size]<<"\t";

      }

      std::cout <<std::endl;

      output_file<<std::endl;

    }

    output_file.close();

  }
  //HessianBands and init function: uses eigensets to get theory bands
  void InitHessianBands( int argc, char *argv[] ){

    std::vector<std::string> expected_args {"fit_dir","output_dir"};

    int num_of_expected_args = expected_args.size();

    if (argc-1<num_of_expected_args){

      std::cout<<"------- passed "<< argc-1<<" arguments but need "

               << num_of_expected_args<<": -------\n";

      for (auto name: expected_args)

        std::cout<<"\t\t"<<name<<"\n";

      std::cout<<"------------------ Quitting. ------------------\n";

      std::exit(0);

    }

    std::string fit_dir(argv[1]);

    home_dir = argv[2];

    para_filename.assign( FileInDirUnique( fit_dir + "/status/", "para" ) );

    gsl_set_error_handler_off();//WARNING: dangerous

    std::filesystem::path home_dir_path(home_dir);

    if ( ! std::filesystem::create_directories(home_dir_path) ){

      std::cout<<"folder "<<home_dir<<" exists. Quitting"<<std::endl;

      std::exit(1);

    }

    //create output directories
    std::string input_dir;

    input_dir = home_dir + "/input/" ;

    std::filesystem::path input_dir_path(input_dir);

    std::filesystem::create_directories(input_dir_path);

    std::string para_dir ;

    para_dir = home_dir + "/status/" ;

    std::filesystem::path para_dir_path(para_dir);

    std::filesystem::create_directories(para_dir_path);

    std::string eigen_dir(para_dir);

    if(0){

      std::string line;
      //cp input file
      std::ifstream input_file (kinematics_filename); //open original input file

      kinematics_filename=hso::read_data::ExtractFname(kinematics_filename); //get file name without path

      std::ofstream input_file_copy( input_dir + "/" + kinematics_filename); //open new file

      while(getline(input_file,line))

        input_file_copy<<line<<"\n";

      input_file .close();

      input_file_copy.close();

    }

    if(0){

      std::string line;
      //cp para file
      std::ifstream input_file (para_filename); //open original para file

      para_filename=hso::read_data::ExtractFname(para_filename); //get file name without path

      std::ofstream input_file_copy( para_dir + "/" + para_filename); //open new file

      while(getline(input_file,line))

        input_file_copy<<line<<"\n";

      input_file.close();

      input_file.clear();

      input_file_copy.close();

      input_file_copy.clear();
      //cp eigensets file
      input_file.open(eigen_filename_global); //open original para file

      eigen_filename_global=hso::read_data::ExtractFname(eigen_filename_global); //get file name without path

      input_file_copy.open(eigen_dir + "/" + eigen_filename_global); //open new file

      while(getline(input_file,line))

        input_file_copy<<line<<"\n";

      input_file .close();

      input_file_copy.close();

    }

    //to use the newly created copies.
    kinematics_filename.assign(input_dir + "/" + kinematics_filename);

    para_filename.assign(para_dir + "/" + para_filename);

    eigen_filename_global.assign( eigen_dir + "/" + eigen_filename_global);

  }

  void HessianBands( ROOT::Minuit2::FCN &theFCN, ROOT::Minuit2::MnUserParameters & upar){
    ///@set flags to store theory/eigen theory in data objects
    hso::store_values_theory=true;//not tread safe

    hso::store_values_stat=true;

    ROOT::Minuit2::fit_iteration_counter=-1;
    ///@fix parameters
    for (auto name : theFCN.para_names_)

      upar.Fix(name);
    ///@read eigensets from file
    std::vector<std::string> eigen_para_names(0);

    std::vector<double> eigen_sets(0);

    int numofeigen=0;

    int numofpara=0;

    ReadEigensets(eigen_filename_global, eigen_para_names,eigen_sets,numofeigen,numofpara);
    ///@set size in data for eigenset results
    for(auto stat: *(theFCN.stat_))

      stat->SetNumOfEigen(numofeigen);//sets numofeigen for each data in stat
    ///@check eigensets in file match in number and names to those in upar (use Minuit error to stop program).
    for(auto name: eigen_para_names) {

      upar.Value(name);

    }
    ///@log file must be set for FCN
    std::ofstream hessian_log_file(home_dir + "/status/hessian.log");

    theFCN.log_file_=&hessian_log_file;
    ///@store theory central values
    ROOT::Minuit2::MnMigrad migrad0(theFCN,upar);

    ROOT::Minuit2::FunctionMinimum min0 = migrad0();//TODO:remove minuit2 dependence. This is a patch
    ///@store eigen theory
    std::cout << "\n# numofeigen="<<numofeigen<<"\n"<<std::endl;

    for(int i=1;i<=numofeigen;i++){

      hso::eigen_index=i;//to store theory in correct location in std::vector

      std::cout << "\n# ++++++++++++++++++++++++ hso::eigen_index="<<hso::eigen_index

                <<"+++++++++++++++++++++++\n"<<std::endl;

      std::vector<double> eigen_set_i(0);

      GetEigenset(eigen_sets,numofpara,i,eigen_set_i);

      int eigen_para_names_size = static_cast<int>(eigen_para_names.size());

      for(int j = 0; j < eigen_para_names_size; j++)

        upar.SetValue(eigen_para_names[j],eigen_set_i[j]);

      ROOT::Minuit2::MnMigrad migrad(theFCN,upar);

      ROOT::Minuit2::FunctionMinimum min = migrad();//TODO:remove minuit2 dependence. This is a patch

    }

    hessian_log_file.close();
    //use eigen theory to compute hessian errors
    for( auto stat: *(theFCN.stat_) )

      stat->EvalHessianError();

  }

}//hso
