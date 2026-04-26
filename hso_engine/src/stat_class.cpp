#include <hso/stat_class.h>
#include <hso/data_class.h>
#include <hso/theory_class.h>
#include <hso/read_data.h>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <map>
#include <algorithm> //std::max
#include <cstdlib>


namespace hso{

  bool store_values_stat=false;

  void Stat::GetBounds(int i, std::vector<double> &bounds){

    std::vector<double>(2).swap(bounds);

    int begin=0;

    int end=0;
    //get index for first element of i-th data set
    for(int j=0;j<i;j++)

      begin+=(*data_)[j]->length_;
    //get index for last element of i-th data set
    end = begin + (*data_)[i]->length_ - 1;

    bounds[0]=begin;

    bounds[1]=end;

  }

  bool Stat::CheckStatFilled(int i){

    Data *data_set=(*data_)[i];

    int stat_values_size = static_cast<int>(data_set -> stat_values_.size());

    return (stat_values_size != 0);

  }

  double Stat::GetTotalValue(int i){

    double output=0;

    std::vector<double> bounds;

    GetBounds(i,bounds);

    for(int j=bounds[0];j<=bounds[1];j++){

      if(point_is_active_[j]) output += stat_values_[j];

    }

    return output;

  }

  double Stat::GetTotalPoints(int i){

    return (*data_)[i]->length_active_;

  }

  double Stat::GetTotalValue(){

    double output=0;

    for(int i=0;i<length_;i++)

      if(point_is_active_[i])

        output += stat_values_[i];

    return output;

  }

  double Stat::GetTotalPoints(){

    return length_active_;

  }

  double Stat::LoadError(Data* data,int i){

    std::vector<double> errors(0);

    double error=0;

    data->GetErr("stat+",i,errors);

    error=errors[0];

    return error;

  }

  void Stat::UpdateIsActive(){

    std::vector<bool> (0).swap(point_is_active_);

    length_active_ = 0;

    for(auto data_set: *data_){

      for(int i=0;i<data_set->length_;i++){

        if (data_set->set_is_active_ &&
            data_set->point_is_valid_[i] &&
            data_set->point_is_active_[i]) {

          point_is_active_.push_back(true);

          length_active_++;

        }

        else {

          point_is_active_.push_back(false);

        }

      }

    }

  }

  void Stat::LoadData(){
    //std::vectors contain always active and inactive (or invalid) entries.
    length_=0;

    length_active_=0;

    std::vector<double> (0).swap(meas_values_);

    std::vector<double> (0).swap( err_values_);

    if(rel_err_sys_pp_!=0) {

      std::cout <<"\n### Adding in quadrature "<<rel_err_sys_pp_*100.0

                <<"% relative error to data objects:\n\n";

      for(auto data_set: *data_)

        std::cout<<"#"<<data_set->name_<<std::endl;

      std::cout<<"### data objects have NOT being modified.\n";

    }

    for(auto data_set: *data_){

      for(int i=0;i<data_set->length_;i++){
        //load meas
        double meas=data_set->meas_values_[i];

        meas_values_.push_back(meas);
        //load error
        double error=LoadError(data_set,i);

        if(rel_err_sys_pp_!=0)

          error=sqrt(pow(error,2) + std::pow(meas*rel_err_sys_pp_,2));

        err_values_ .push_back(error);

        length_++;

      }

    }

    UpdateIsActive();
    //set stat_values_ to correct size
    std::vector<double> (length_,0).swap(stat_values_);

  }

  void Stat::SetRange(std::string varname, double min, double max){

    for(auto data_set: *data_) {

      data_set->SetRange(varname,min,max);

    }

    UpdateIsActive();

  }

  double Stat::Eval(double meas,double error,double theory,void *){

    return std::pow( (meas-theory)/error , 2 );

  }

  double Stat::EvalPenalties(void *){

    std::cout<<"\n###Error: using Stat:EvalPenalties but this (base) class"

             <<" is not meant to compute penalties. Aborting\n";

    std::exit(0);

    return 0;

  }

  double Stat::operator()(void *para,void *pointer,bool store_values){//stat_values_ stored only if store_values = true
    //usually the total chi2
    double stat_total=0.0;

    int index=0;
    //compute theory for every active point for every pointer in Data.
    for(auto data_set: *data_) {
      //eval theory pts and store in temp std::vector. For inactive data points, theory = NAN
      std::vector<double> theoryvalues_internal=theory_->operator()(*data_set,para);

      std::vector<double> statvalues_internal(data_set->length_);

      for(int i=0;i<data_set->length_;i++) {

        double stat_i=0;

        double theory_i = theoryvalues_internal[i];

        if(point_is_active_[index]) {

          stat_i = Eval(meas_values_[index],err_values_[index],theory_i,pointer);

          stat_total += stat_i;

        }
        ///WARNING: to store statvalues into data. NOT tread safe
        if(store_values) {

          statvalues_internal[i] = stat_i;

        }

        index++;

      }
      ///WARNING: to store statvalues into data. NOT tread safe
      if(store_values) data_set->stat_values_=statvalues_internal;

    }

    if(penalties_exist_) stat_total += EvalPenalties(pointer);

    return stat_total;

  }

  void Stat::SetNumOfEigen(int numofeigen){

    if(numofeigen%2==0){//for case where both directions of eigen std::vectors are considered

      for(auto data_set: *data_) {

        data_set->number_of_eigen_=numofeigen;

        std::vector<double> (data_set->number_of_eigen_*data_set->length_/2,NAN).swap(data_set->theory_values_eigen_plus_);

        std::vector<double> (data_set->number_of_eigen_*data_set->length_/2,NAN).swap(data_set->theory_values_eigen_minus_);

      }

    }

    else {

      std::cout<<"\n#Error: must have even number of eigensets. Only +/-"

               <<" version implemented. Aborting.\n";

      std::exit(0);

    }

  }

  void Stat::EvalHessianError() {//stored in data objects

    for(auto data_set: *data_){

      if(data_set->set_is_active_){

        std::vector<double>(data_set->length_,NAN).swap(data_set->theory_values_err_plus_);

        std::vector<double>(data_set->length_,NAN).swap(data_set->theory_values_err_minus_);
        //initialize error containers
        for(int i=0;i<data_set->length_;i++)if(data_set->point_is_active_[i]){

          data_set->theory_values_err_plus_ [i]=0;

          data_set->theory_values_err_minus_[i]=0;

        }

        for(int eigen_i=1;eigen_i<=data_set->number_of_eigen_;eigen_i+=2){

          std::vector<double> Plus(data_set->GetEigenTheory(eigen_i));

          std::vector<double> Minus(data_set->GetEigenTheory(eigen_i+1));

          std::vector<double> Central(data_set->theory_values_);

          for(int i=0;i<data_set->length_;i++)if(data_set->point_is_active_[i]){//add in quadrature

            //eq 13 from lhapdf paper: DOI: 10.1140/epjc/s10052-015-3318-8
            // double plus = Plus [i];
            // double minus = Minus[i];
            // double sigma2 = std::pow(Plus[i]-Minus[i],2);
            //
            //
            // data_set->theory_values_err_plus_ [i] += sigma2;
            // data_set->theory_values_err_minus_[i] += sigma2;

            // eqs 11 & 12 from lhapdf paper: DOI: 10.1140/epjc/s10052-015-3318-8
            double plus = Plus [i]-Central[i];

            double minus = Minus[i]-Central[i];

            double sigma2plus = std::pow(std::max( plus, minus),2);

            double sigma2minus = std::pow(std::max(-plus,-minus),2);

            data_set->theory_values_err_plus_ [i] += sigma2plus;

            data_set->theory_values_err_minus_[i] += sigma2minus;

          }

        }

        for(int i=0;i<data_set->length_;i++)if(data_set->point_is_active_[i]){
          // eq 13 from lhapdf paper : DOI: 10.1140/epjc/s10052-015-3318-8
          // data_set->theory_values_err_plus_ [i] = std::sqrt( data_set->theory_values_err_plus_ [i])/2.0;
          //
          // data_set->theory_values_err_minus_[i] = std::sqrt( data_set->theory_values_err_minus_[i])/2.0;

          // eqs 11 & 12 from lhapdf paper : DOI: 10.1140/epjc/s10052-015-3318-8
          data_set->theory_values_err_plus_ [i] = std::sqrt( data_set->theory_values_err_plus_ [i]);

          data_set->theory_values_err_minus_[i] = std::sqrt( data_set->theory_values_err_minus_[i]);

        }

      }

    }

    hessian_error_present_=true;

  }

  bool Stat::Write(std::string fname_out,int i){//WARNING: not thread safe.

    bool wrote_file=false;

    Data* data_set=(*data_)[i];

    if(CheckStatFilled(i)){

      if(read_data::FileExists(fname_out)){

        std::cout<<"File "<<fname_out<<" exists. Aborting."<<std::endl;

        std::exit(1);

      };

      if(data_set->set_is_active_){

        std::ofstream file(fname_out);

        if(file.good()){

          double chi2=GetTotalValue (i);

          int pts=GetTotalPoints(i);

          file<<"#\n#\n";

          file<<"#pts="<<pts<<"\n";

          file<<"#chi^2="<<chi2<<"\n";

          file<<"#chi^2/pts="<<chi2/pts<<"\n\n";

          for(int j=0;j<data_set->length_;j++){

            if(data_set->point_is_active_[j]){

              std::vector<double> data_line(0);

              data_line=data_set->Get(j,false);

              for(auto val: data_line)

                file<<val<<"\t";

              file<<data_set->theory_values_[j]<<"\t";

              if(data_set->normalize_)

                file<<data_set->norm_values_ [j]<<"\t";

              else if(data_set->shift_)

                file<<data_set->shift_values_[j]<<"\t";

              //stat value point by point
              file<<data_set->stat_values_[j]<<"\t";

              if(hessian_error_present_) {

                file<<data_set->theory_values_err_plus_[j]<<"\t";

                file<<data_set->theory_values_err_minus_[j]<<"\t";

              }

              file<<std::endl;

            }

          }

          file.close();

          wrote_file=true;

        }

      }

    }

    else {

      std::cout<<"#data object '"<<data_set->name_

               <<"' has empty values in stat_values_"<<std::endl;

    }

    return wrote_file;

  }

  StatN::StatN(std::string name_in, Theory* theory_pointer, std::vector<Data*> &data_pointers, double rel_err_sys_pp_in, double delta_N__in):

  Stat(name_in, theory_pointer, data_pointers, rel_err_sys_pp_in), delta_N_(delta_N__in) {

    for(auto set: *data_) {

      set->normalize_=true;

      std::vector<double>(set->length_,1.0).swap(set->norm_values_);

    }

    penalties_exist_=true;

  }

  double StatN::Eval(double meas,double error,double theory,void *pointer){

    std::map<std::string,double> norm_map = *static_cast<std::map<std::string,double>**>(pointer)[0];

    double norm = norm_map[name_];

    return std::pow( (meas-norm*theory)/error, 2);

  }

  double StatN::EvalPenalties(void *pointer){

    std::map<std::string,double> norm_map = *static_cast<std::map<std::string,double>**>(pointer)[0];

    double norm=norm_map[name_];

    return std::pow( (1.0-norm)/delta_N_, 2);

  }

  StatNMin::StatNMin(std::string name_in,Theory* theory_pointer,std::vector<Data*> &data_pointers,double rel_err_sys_pp_in,double delta_N__in):

  Stat(name_in,theory_pointer,data_pointers,rel_err_sys_pp_in),delta_N_(delta_N__in) {

    for(auto set: *data_) {

      set->normalize_=true;

      std::vector<double>(set->length_,1.0).swap(set->norm_values_);

    }

    penalties_exist_=true;

  };
  //TODO: need to add code to save chi2 into data
  double StatNMin::operator()(void *para,void *,bool store_values){

    int index =0;

    std::vector<double>TheoryValues_internal(length_);

    double s1=0,s2=0,norm=1.0,stat_total=0.0;
    //compute theory for every active point for every pointer in Data.
    for(auto data_set: *data_) {
      //eval theory pts and store in temp std::vector. For inactive data points, theory = NAN
      std::vector<double> theoryvalues_internal=theory_->operator()(*data_set,para);

      for(int i=0;i<data_set->length_;i++) {

        TheoryValues_internal[index] = theoryvalues_internal[i];

        index++;

      }

    }
    //compute quantities needed to construct chi2 minimized respect to normalization.
    for(int i=0;i<length_;i++) if(point_is_active_[i]){

      double meas =meas_values_[i];

      double error = err_values_[i];

      double theory =TheoryValues_internal[i];

      s1 += meas*theory/pow(error,2);

      s2 += std::pow(theory/error,2);

    }
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    //NOTE if N multiplies theory                ///////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    if(!fix_norm_) norm = (1.0 + std::pow(delta_N_,2)*s1)/(1.0 + std::pow(delta_N_,2)*s2);
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    //NOTE if N divides theory (as in DY pheno paper) //
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    // if(!fix_norm){
    //
    //
    // double norm4=0.25 - std::sqrt(0.25 + (pow(2,0.3333333333333333)*(s1*std::pow(delta_N_,2) - 4*s2*std::pow(delta_N_,2)))/
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4) +
    // std::sqrt(-4*std::pow(3*s1*std::pow(delta_N_,2) - 12*s2*std::pow(delta_N_,2),3) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4),2)),0.3333333333333333) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4) +
    // std::sqrt(-4*std::pow(3*s1*std::pow(delta_N_,2) - 12*s2*std::pow(delta_N_,2),3) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4),2)),0.3333333333333333)/
    // (3.*std::pow(2,0.3333333333333333)))/2. +
    // std::sqrt(0.5 - (pow(2,0.3333333333333333)*(s1*std::pow(delta_N_,2) - 4*s2*std::pow(delta_N_,2)))/
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4) +
    // std::sqrt(-4*std::pow(3*s1*std::pow(delta_N_,2) - 12*s2*std::pow(delta_N_,2),3) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4),2)),0.3333333333333333) -
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4) +
    // std::sqrt(-4*std::pow(3*s1*std::pow(delta_N_,2) - 12*s2*std::pow(delta_N_,2),3) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4),2)),0.3333333333333333)/
    // (3.*std::pow(2,0.3333333333333333)) -
    // (1 - 8*s1*std::pow(delta_N_,2))/
    // (4.*std::sqrt(0.25 + (pow(2,0.3333333333333333)*(s1*std::pow(delta_N_,2) - 4*s2*std::pow(delta_N_,2)))/
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4) +
    // std::sqrt(-4*std::pow(3*s1*std::pow(delta_N_,2) - 12*s2*std::pow(delta_N_,2),3) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4),2)),
    // 0.3333333333333333) + std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4) +
    // std::sqrt(-4*std::pow(3*s1*std::pow(delta_N_,2) - 12*s2*std::pow(delta_N_,2),3) +
    // std::pow(-27*s2*std::pow(delta_N_,2) + 27*std::pow(s1,2)*std::pow(delta_N_,4),2)),
    // 0.3333333333333333)/(3.*std::pow(2,0.3333333333333333)))))/2.;
    //
    // if(norm4==norm4) norm=norm4;
    // // else {
    // //
    // // std::cout<<"##unused s1="<<s1<<" s2="<<s2<<": data="<<(*data_)[0]->name<<std::endl;
    // //
    // //
    // // // std::exit(0);
    // //
    // // }
    //
    // }
    /////////////////////////////////////////////////
    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    //compute chi2
    for(int i=0;i<length_;i++) if(point_is_active_[i]){

      double meas =meas_values_[i];

      double error =err_values_[i];

      double theory =TheoryValues_internal[i];

      double stat_i = std::pow( (meas-theory*norm)/error , 2 );//norm multiplies theory

      stat_total += stat_i;
      //store stat vals if flag is true
      if(store_values) stat_values_[i]=stat_i;

    }
    //add penalty term from normalization (EvalPenalties is not used here)
    if(penalties_exist_&&s1!=0&&s2!=0) {//NOTE: PATCH: s1=s2=0 when there are no active poiints

      stat_total += std::pow( (norm-1.0)/delta_N_ , 2 );

    }

    if(store_values){

      int data_size = static_cast<int>(data_ -> size());

      for(int i=0; i < data_size; i++){

        Data *data_set=(*data_)[i];

        std::vector<double>(0).swap(data_set->stat_values_);

        std::vector<double>(0).swap(data_set->norm_values_);

        std::vector<double> bounds_j;

        GetBounds(i,bounds_j);

        for(int j=bounds_j[0];j<=bounds_j[1];j++){

          data_set->stat_values_.push_back(stat_values_[j]);

          data_set->norm_values_.push_back(norm);

        }

      }

    }

    return stat_total;

  }

}//hso

