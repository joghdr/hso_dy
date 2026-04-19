#include <hso/theory_class.h>
#include <hso/cuba_settings.h>
#include <hso/data_class.h>
#include <hso/read_data.h>
#include "cuba.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <cstdlib>


typedef int (*cuba_function)(const int *ndim, const cubareal xx[], const int *ncomp,

                             cubareal ff[], void *P);

namespace hso{

  bool store_values_theory=false;

  int eigen_index=0;

  void hso::Theory::Info(){

  };
  //avg val var min and max for varint or var binavg
  bool hso::Theory::GetVarValues(hso::Data &data, int i, std::vector<double> &values,

                                 std::vector<double> &min, std::vector<double> &max,

                                 std::vector<double> &binavg){

    using hso::read_data::ColumnType;

    std::vector<double>(0).swap(values);

    std::vector<double>(0).swap( min);

    std::vector<double>(0).swap( max);

    std::vector<double>(0).swap(binavg);

    int var_names_size = static_cast<int>(var_names_.size());

    bool found = false;

    for(int j=0;j < var_names_size; j++){

      std::vector<double> store(0);

      found = data.GetVar(var_names_[j],i,store);

      VariableType behave=var_behave_[j];

      if(!found)

        found=data.GetVarOther(var_names_[j],store);

      if(found){

        auto data_var_type = static_cast<ColumnType>(data.var_type_[var_names_[j]]);

        if(behave==VariableType::Value) {

          if(data_var_type==ColumnType::VariableAvg||data_var_type==ColumnType::VariableBin)

            values.push_back(store[0]);

          else if(data_var_type==ColumnType::VariableRange)

            values.push_back( (store[0]+store[1])/2.0);

        }

        else if (behave==VariableType::Integration) {

          if(data_var_type==ColumnType::VariableRange) {

            min.push_back(store[0]);

            max.push_back(store[1]);

          }

          else if (data_var_type==ColumnType::VariableBin) {

            min.push_back(store[1]);

            max.push_back(store[2]);

            binavg.push_back(store[0]);

          }

          else {

            std::cout<<"#Error: "<<__FILE__<<":"<<__LINE__

                     <<": requested integration for '"<<var_names_[j]

                     <<"' but variable type in data object '"<<data.name_

                     <<"' is not compatible. Aborting."<<std::endl;

            std::exit(0);

          }

        }

        else if (behave==VariableType::BinAvg){

          if(data_var_type==ColumnType::VariableRange) {

            min.push_back(store[0]);

            max.push_back(store[1]);

          }

          else if(data_var_type==ColumnType::VariableBin) {

            min.push_back(store[1]);

            max.push_back(store[2]);

            binavg.push_back(store[0]);

          }

          else {

            std::cout<<"#Error: "<<__FILE__<<":"<<__LINE__

                     <<": requested bin average for '"<<var_names_[j]

                     <<"' but variable type in data object '"<<data.name_

                     <<"' is not compatible. Aborting."<<std::endl;

            std::exit(0);

          }

        }
        //NOTE if var_values contains 2 numbers, it means variable is int type so has
        //     only min and max. Thus, [0]=min and computation is incorrect. User must
        //     make sure the variable requested exist with the corresponding type in
        //     data file. UpDATE this now should work in all cases or show err message
        //     and abort.
      }

      else if(behave==VariableType::Undetermined){

        if(print_warning_undetermined_>0){

          std::cout<<"#Warning: "<<var_names_[j]<<" not found in "<<data.name_

                   <<", but set to behave as 'VariableType::Undetermined'. This is"

                   <<" intended for integration where limits depend on other variables."

                   <<" Check this is implemented in integrand."<<std::endl;

          print_warning_undetermined_--;

        }

      }

      else {

        std::cout<<"#Error: "<<__FILE__<<":"<<__LINE__

                 <<":"<<var_names_[j]<<" not found in "<<data.name_

                 <<". Aborting"<<std::endl;

        std::exit(0);

      }

    }

    return found;

  };

  double hso::Theory::Eval(hso::Data &data,int i,void *para){

    void *scales =static_cast<void**>(para)[0];

    void *Ftilde_a_para=static_cast<void**>(para)[1];

    void *Ftilde_b_para=static_cast<void**>(para)[2];

    void *Ktildepara =static_cast<void**>(para)[3];

    int dim=0;

    int cuba_dim=0;
    //avg variables, min & max for integration
    std::vector<double> values(0),min_v(0),max_v(0),binavg(0);

    GetVarValues(data,i,values,min_v,max_v,binavg);

    dim = static_cast<int>(min_v.size());

    cuba_dim=dim+1;//adds always one to account for bT or kT integration (FT or convolution)
    //add variables set to VariableType::Undetermined
    for (auto entry: var_behave_)

      if (entry==VariableType::Undetermined)

        cuba_dim++;
    ///@check: only multidim integral allowed. Abort if not true.
    if(cuba_dim<2){

      std::cout<<"Error: requested to integrate "<<dim

               <<" variables in multi-dimensional integration. Aborting."<<std::endl;

      std::exit(1);

    }

    std::vector<double> csecvars(values.size());

    int values_size = static_cast<int>(values.size());

    for(int j = 0; j < values_size; j++){

      csecvars[j] = values[j];

    }

    std::vector<double> min(dim);

    std::vector<double> max(dim);

    for(int j = 0; j < dim; j++){

      min[j] = min_v[j];

      max[j] = max_v[j];

    }

    std::vector<VariableType> behavior(var_behave_.size());

    int var_behave_size = static_cast<int>(var_behave_.size());
    //TODO check if behave is needed here. Remove loop seem superfluos
    for(int j = 0; j < var_behave_size; j++){

      behavior[j] = var_behave_[j];

    }

    void *P[]={
      &behavior,
      &csecvars,
      &min,&max,
      &binavg,
      scales,
      Ftilde_a_para,
      Ftilde_b_para,
      Ktildepara
    };
    /// CUBA related vars and functions
    int nregions=0,neval=0;

    int fail=0;

    double integral_cuhre[cuba_dim], error[cuba_dim], prob[cuba_dim];

    using namespace hso::cuba_default;

    Cuhre(cuba_dim, 1,cuba_integrand_, P, kCubaNVec,

          kCubaEpsRel, kCubaEpsAbs, CubaVerbose ,

          kCubaMinEval, kCubaMaxEval, kCubaKey,

          NULL, NULL, //NOTE null pointers are 'cuba_statefile', 'cuba_spin'

          &nregions, &neval, &fail, integral_cuhre, error, prob);

    return integral_cuhre[0];

  };

  std::vector<double> hso::Theory::operator()(hso::Data &data,void *para,

                                              bool store_values,int eigen_i){//eigen_i=1,2,3,...

    std::vector<double> TheoryValues_internal(data.length_,NAN);

    if( store_values && (eigen_i>=1) && data.number_of_eigen_==0 ){

      std::cout<<"#Error: requested to store theory in eigen_i="<<eigen_i

               <<" but data.number_of_eigen_="<<data.number_of_eigen_<<", for data '"

               <<data.name_<<"'. Aborting."<<std::endl;

      std::exit(0);

    }

    if( store_values && (eigen_i>=1) && data.number_of_eigen_%2==1 ) {

      std::cout<<"#Error: requested to store theory in eigen_i="<<eigen_i

               <<" but data.number_of_eigen_="<<data.number_of_eigen_<<", for data '"

               <<data.name_<<"'. Number of eigensets must be even, since only +/-"

               <<" version is implemented. Aborting."<<std::endl;

      std::exit(0);

    }

    for(int i=0;i<data.length_;i++) {

      if(data.point_is_active_[i])

        TheoryValues_internal[i]=Eval(data,i,para);

    }
    //to store theory as central values
    //eigen_i=1,2,3,...
    if(store_values&&eigen_i<1) data.theory_values_ = TheoryValues_internal;
    //to store theory as eigen sets
    if(data.number_of_eigen_!=0&&(data.number_of_eigen_%2)==0&&eigen_i>=1){//eigen_i=1,2,3,...
      //initialize std::vectors that store eigen theory if they have size!=numofeigen*data.length_/2
      int theory_values_eigen_plus = static_cast<int>(data.theory_values_eigen_plus_.size());

      if(theory_values_eigen_plus != data.number_of_eigen_ * data.length_/2 )

        std::vector<double> (data.number_of_eigen_*data.length_/2,NAN).swap(data.theory_values_eigen_plus_);

      int theory_values_eigen_minus = static_cast<int>(data.theory_values_eigen_minus_.size());

      if(theory_values_eigen_minus != data.number_of_eigen_*data.length_/2)

        std::vector<double> (data.number_of_eigen_*data.length_/2,NAN).swap(data.theory_values_eigen_minus_);

      if( ((eigen_i - 1) % 2) == 0 ) {//eigen_i=1,3,5,...

        int data_length = data.length_;

        for(int i = 0; i < data_length; i++){

          int index= i + data_length * ( eigen_i - 1 )/2;//eigen_i=1,3,5,...

          data.theory_values_eigen_plus_ [index]=TheoryValues_internal[i];

        }

      }

      if( ((eigen_i - 1) % 2) == 1 ) {//eigen_i=2,4,6,...

        int data_length = data.length_;

        for(int i=0;i<data_length;i++){

          int index= i + data_length* ( eigen_i - 2 )/2;//eigen_i=2,4,6,...

          data.theory_values_eigen_minus_[index]=TheoryValues_internal[i];

        }

      }

    }

    return TheoryValues_internal;

  };

  bool hso::Theory::Write(hso::Data &data,std::string fname_out,bool alldata) {//NOTE: not thread safe.

    bool wrote_file=false;

    if(hso::read_data::FileExists(fname_out)){

      std::cout<<"File "<<fname_out<<" exists. Aborting."<<std::endl;

      std::exit(1);

    };

    if(data.set_is_active_||alldata){

      std::ofstream file(fname_out);

      if(file.good()){

        for(int i=0;i<data.length_;i++){

          if(data.point_is_active_[i]||alldata){

            std::vector<double> data_line(0);

            data_line=data.Get(i,false);//flag: check (if active)

            for(auto val: data_line)

              file<<val<<"\t";

            file<<data.theory_values_[i]<<"\t";

            file<<std::endl;

          }

        }

        file.close();

        wrote_file=true;

      }

      else {

        std::cout<<"Error: Could not open '"<< fname_out <<"'. Aborting."

                 <<std::endl;std::exit(0);

      }

    }

    return wrote_file;

  }

}//hso


