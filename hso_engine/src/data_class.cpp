#include <hso/data_class.h>
#include <hso/read_data.h>
#include <vector>
#include <string>
// #include <cstddef> //std::size_t
#include <utility>
#include <map>
#include <iostream>
#include <stdexcept>//std::out_of_range
#include <initializer_list>
#include <tuple>
#include <fstream>
#include <cstdlib>
#include <cmath>
///@description:
///1) Needs data files prepared in a 'rectangular' grid. Input file should have comments starting with '#'
/// to be ignored. Number of lines and columns are determined automatically. 
/// 
///2) Empty entries in bins, value and error are set to BLANK (defined in 'include/read_data.h').
///3) std::vector<bool> Exists determines if there is a data point. Set to false in cases 2)
///4) std::vector<bool> Active to decide which data is gonna be used in a loop, e.g. fit with data cuts
///5) std::vector<double> Data contains data values. One dim_ensional even for multidim_ensional data.
///6) std::vector<double> Errors contains errors for a given data point. Errors can be longer than Data if 
/// different types of errors present, e.g. if there are stat and systcorr
// Errors will be twice as long as Data.
namespace hso{
  //init
  void Data::UpdateActive(){

    length_active_=0;

    for(int i=0;i<length_;i++ )

      if(point_is_valid_[i]&&point_is_active_[i])

        length_active_++;

    if(length_active_==0)

      set_is_active_=false;

    else

      set_is_active_=true;

  }

  void Data::Init(std::string fname_in){

    std::string name_f(fname_in);

    std::string name_d(fname_in);

    std::size_t pos=name_f.rfind("/");

    if(pos!=std::string::npos) {

      name_f.erase(0,pos+1);

      name_d.erase(pos+1,name_d.length()-pos);

    }

    else {

      name_d.assign("./");

    }

    file_name_=name_f;

    dir_name_=name_d;

    pos=name_f.rfind(".");

    if (pos != std::string::npos) {

      name_f.erase(pos,name_f.length()-pos);

      name_ = name_f;

    }

    else

      name_ = name_f;

    std::string key=read_data::LoadFile(dir_name_ + file_name_, data_lines_,

                                        comment_lines_, dim_, var_bin_names_,

                                        var_int_names_, var_avg_names_, meas_names_,

                                        err_names_, input_column_number_);

    std::map<std::string,int> dictionary;

    dictionary=read_data::GetDictionary(key, var_bin_names_, var_int_names_,

                                        var_avg_names_, meas_names_, err_names_);

    while (read_data::ReadLineValues(dictionary, data_lines_, var_bin_names_,

      var_int_names_,var_avg_names_,meas_names_,err_names_,

      var_bin_avg_,var_bin_min_,var_bin_max_,var_int_min_,

      var_int_max_,var_avg_,meas_values_,err_values_)
    );

    number_of_var_bin_ = var_bin_names_.size();

    number_of_var_int_ = var_int_names_.size();

    number_of_var_avg_ = var_avg_names_.size();

    number_of_meas_ = 1;

    number_of_err_=err_names_.size();
    //maps for easy acess to values
    for(int j=0;j<number_of_var_bin_;j++)

      varbin_j_.insert({var_bin_names_[j],j});

    for(int j=0;j<number_of_var_int_;j++)

      varint_j_.insert({var_int_names_[j],j});

    for(int j=0;j<number_of_var_avg_;j++)

      varavg_j_.insert({var_avg_names_[j],j});

    for(int k=0;k<number_of_err_ ;k++)

      err_k_.insert({err_names_[k],k});
    //set active length, valid and active
    length_ = meas_values_.size();
    //mark lines that are not valid
    std::vector<bool> (length_,false).swap(point_is_valid_);

    for(int i=0;i<length_;i++ ) if(meas_values_[i]==meas_values_[i]) point_is_valid_[i]=true;
    //mark all valid bins as active and count active bins
    std::vector<bool> (length_,false).swap(point_is_active_);

    length_active_=0;

    for (int i=0;i<length_;i++ ) {

      if(point_is_valid_[i]) {

        point_is_active_[i]=true;

        length_active_++;

        set_is_active_=true;

      }

    }

    read_data::SearchOtherKeys(comment_lines_, other_vars_);
    //Sep 2023
    std::vector<double> (meas_values_.size(),NAN).swap(theory_values_);

    std::vector<double> (meas_values_.size(),1.0).swap(norm_values_);

    std::vector<double> (meas_values_.size(),0.0).swap(shift_values_);

    normalize_=false;

    shift_=false;

    SetBehavior();

  }

  //methods
  std::vector<double> Data::Get(int i,bool check,bool omit_errors){

    std::vector<double> output(0);

    bool use_i=true;

    if(check)

      use_i=point_is_active_[i]&&point_is_valid_[i];

    if(use_i){
      //varbin
      for (int j=0;j<number_of_var_bin_;j++) {

        int index=i*number_of_var_bin_+j;

        output.push_back(var_bin_avg_[index]);

        output.push_back(var_bin_min_[index]);

        output.push_back(var_bin_max_[index]);

      }
      //varint
      for (int j=0;j<number_of_var_int_;j++){

        int index=i*number_of_var_int_+j;

        output.push_back(var_int_min_[index]);

        output.push_back(var_int_max_[index]);

      }
      //varavg
      for (int j=0;j<number_of_var_avg_;j++){

        int index=i*number_of_var_avg_+j;

        output.push_back(var_avg_[index]);

      }
      //meas
      output.push_back(meas_values_[i]);

      //err
      if (!omit_errors)

        for (int j=0;j<number_of_err_;j++){

          int index=i*number_of_err_+j;

          output.push_back(err_values_[index]);

        }

    }

    else{

      if (!point_is_valid_[i])

        std::cout<<"#"<<"i="<<i<<": values are not valid."<<std::endl;

      if (!point_is_active_[i])

        std::cout<<"#"<<"i="<<i<<": values are not active."<<std::endl;

    }

    return output;

  }
  /// read variable values, meas and errors
  bool Data::GetVar(std::string name,int i,std::vector<double> &output){

    bool found=true;

    int j=-1;

    int index=-1;

    try {

      j=varbin_j_.at(name);

      index=i*number_of_var_bin_+j;

      output.push_back(var_bin_avg_[index]);

      output.push_back(var_bin_min_[index]);

      output.push_back(var_bin_max_[index]);

    }

    catch (const std::out_of_range& oor_var_bin) {

      try {

        j=varint_j_.at(name);

        index=i*number_of_var_int_+j;

        output.push_back(var_int_min_[index]);

        output.push_back(var_int_max_[index]);

      }

      catch (const std::out_of_range& oor_var_int) {

        try {

          j=varavg_j_.at(name);

          index=i*number_of_var_avg_+j;

          output.push_back(var_avg_[index]);

        }

        catch (const std::out_of_range& oor_var_avg) {

          found=false;

        }

      }

    }

    return found;

  }

  void Data::GetVar(std::initializer_list<std::string> keys,int i,std::vector<double> &output){//NOTE: not so useful, warning!!

    for(auto name: keys)GetVar(name,i,output);

  }

  void Data::GetErr(std::string name,int i,std::vector<double> &output){

    int k=-1;

    int index=-1;

    try {

      k=err_k_.at(name);

      index=i*number_of_err_+k;

      output.push_back(err_values_[index]);

    }

    catch (const std::out_of_range& oor) {

      std::cout<<"#"<<name<<" is not a type of error."<<std::endl;

    }

  }

  void Data::GetErr(std::initializer_list<std::string> keys,int i,std::vector<double> &output){

    for(auto name: keys)

      GetErr(name,i,output);

  }

  double Data::GetMeas(int i){

    return meas_values_[i];

  }

  bool Data::GetVarOther(std::string name,std::vector<double> &output){

    using hso::read_data::ColumnType;

    bool found=true;

    std::vector<double>().swap(output);

    try {

      std::tuple<int,double,double,double> other_vars(other_vars_.at(name));
      // int type=std::get<0>(other_vars);
      auto type=static_cast<ColumnType>(std::get<0>(other_vars));

      switch(type){

        case ColumnType::VariableBin:

          output.push_back(std::get<1>(other_vars));

          output.push_back(std::get<2>(other_vars));

          output.push_back(std::get<3>(other_vars));

          break;

        case ColumnType::VariableRange:

          output.push_back(std::get<1>(other_vars));

          output.push_back(std::get<2>(other_vars));

          break;

        case ColumnType::VariableAvg:

          output.push_back(std::get<1>(other_vars));

          break;

        default:

          break;

      }

    }

    catch(const std::out_of_range& oor){

      std::cout<<"#"<<name<<" is not present in other_vars_."<<std::endl;

      found=false;

    }

    return found;

  }
  ///@set data ranges
  void Data::CutMin(std::string name,double min,bool action){//false to make inactive, true to make active

    for(int i=0;i<length_;i++)if(point_is_valid_[i]){

      std::vector<double> vals;

      if(GetVar(name,i,vals)||GetVarOther(name,vals)){

        bool out_of_range=( vals[0]<min );

        if(out_of_range) {point_is_active_[i]=action; }

      }

    }

  }

  void Data::CutMax(std::string name,double max,bool action){//false to make inactive, true to make active

    for(int i=0;i<length_;i++)if(point_is_valid_[i]){

      std::vector<double> vals;

      if(GetVar(name,i,vals)||GetVarOther(name,vals)){

        bool out_of_range=( vals[0]>max );

        if(out_of_range) point_is_active_[i]=action;

      }

    }

  }

  void Data::SetRange(std::string name,double min, double max){

    CutMin(name, min);

    CutMax(name, max);

    UpdateActive();

  }
  //write
  bool Data::Write(std::string fname_out,bool alldata){

    bool wrote_file=false;

    if(read_data::FileExists(fname_out)){

      std::cout<<"\n#File "<<fname_out<<" exists. Aborting."<<std::endl;

      std::exit(1);

    }

    if(set_is_active_||alldata){

      std::ofstream file(fname_out);

      if(file.good()){

        for(int i=0;i<length_;i++){

          if(point_is_active_[i]||alldata){

            std::vector<double> data_line(0);

            data_line=Get(i,false);

            for(auto val: data_line) file<<val<<"\t";

            file<<std::endl;

          }

        }

        file.close();

        wrote_file=true;

      }

    }

    return wrote_file;

  }

  std::vector<double> Data::GetEigenTheory(int eigen_i){//1,3,5,.. for plus, 2,4,6,.. for minus

    if(number_of_eigen_<eigen_i) {

      std::cout<<"\n#Error: requested theory values for "<<eigen_i

      <<"-th eigenset, but currently there are number_of_eigen_="

      <<number_of_eigen_<<"in data '"<<name_<<"'. Aborting."<<std::endl;

      std::exit(0);

    }

    int current_size_plus = static_cast<int>(theory_values_eigen_plus_.size());

    if( current_size_plus != (length_ * number_of_eigen_ / 2)){

      std::cout<<"\n#Error: size of theory_values_eigen_plus_ should be "

      <<length_*number_of_eigen_/2<<"\n# theory_values_eigen_plus_.size() = "

      <<theory_values_eigen_plus_.size()<<"\n# Aborting."<<std::endl;

      std::exit(0);

    }

    int current_size_minus = static_cast<int>(theory_values_eigen_minus_.size());

    if(current_size_minus != (length_ * number_of_eigen_ / 2)){

      std::cout<<"\n#Error: size of theory_values_eigen_minus_ should be "

      <<length_*number_of_eigen_/2<<"\n# theory_values_eigen_minus_.size()="

      <<theory_values_eigen_minus_.size()<<"\n# Aborting."<<std::endl;

      std::exit(0);

    }

    std::vector<double> output(0);

    if (((eigen_i-1)%2) == 0 ) {//eigen_i=1,3,5,...

      int index_min = length_* ( eigen_i - 1 )/2;

      int index_max = length_ - 1 + length_* ( eigen_i - 1 )/2;

      for(int i=index_min;i<=index_max;i++){

        output.push_back(theory_values_eigen_plus_[i]);

      }

    }

    else if (((eigen_i-1)%2 ) == 1) {//eigen_i=2,4,6,...

      int index_min = length_* ( eigen_i - 2 )/2;

      int index_max = length_ - 1 + length_* ( eigen_i - 2 )/2;

      for(int i=index_min;i<=index_max;i++){

        output.push_back(theory_values_eigen_minus_[i]);

      }

    }

    int output_size = static_cast<int>(output.size());

    if(output_size != length_){

      std::cout<<"\n#Error: extracted values from theory_values_eigen_plus_(Minus)"

      <<" are of different length than length_.\n"<<"# length_="<<length_

      <<"\tsize of extracted="<<output.size()<<"\n# Aborting."<<std::endl;

      std::exit(0);

    }

    return output;

  }

  void Data::Info(){

    std::cout<<"#name = "<<name_ <<std::endl;

    std::cout<<"#dir_name_ = "<<dir_name_ <<std::endl;

    std::cout<<"#file_name_ = "<<file_name_ <<std::endl;

    std::cout<<"#key = "<<key_ <<std::endl;

    std::cout<<"#set_is_active_ = "<< set_is_active_ <<std::endl;

    std::cout<<"#length_ = "<< length_ <<std::endl;

    std::cout<<"#length_active_ = "<< length_active_ <<std::endl;

    std::cout<<"#number_of_var_bin_ = "<<number_of_var_bin_<<": ";

    for(auto name:var_bin_names_)

      std::cout<<name<<"\t";

    std::cout<<std::endl;

    std::cout<<"#number_of_var_int_ = "<<number_of_var_int_<<": ";

    for(auto name:var_int_names_)

      std::cout<<name<<"\t";

    std::cout<<std::endl;

    std::cout<<"#number_of_var_avg_ = "<<number_of_var_avg_<<": ";

    for(auto name:var_avg_names_)

      std::cout<<name<<"\t";

    std::cout<<std::endl;

    std::cout<<"#number_of_meas_ = "<<number_of_meas_ <<": ";

    for(auto name:meas_names_ )

      std::cout<<name<<"\t";

    std::cout<<std::endl;

    std::cout<<"#number_of_err_ = "<<number_of_err_ <<": ";

    for(auto name:err_names_ )

      std::cout<<name<<"\t";

    std::cout<<std::endl;

    std::cout<<"#rows="<<dim_[0]<<" cols="<<dim_[1]<<std::endl;

    std::cout<<"#varbin_j_: ";

    for(auto entry: varbin_j_)

      std::cout<<entry.first<<" --> "<<entry.second<<"\t";

    std::cout<<std::endl;

    std::cout<<"#varint_j_: ";

    for(auto entry: varint_j_)

      std::cout<<entry.first<<" --> "<<entry.second<<"\t";

    std::cout<<std::endl;

    std::cout<<"#varavg_j_: ";

    for(auto entry: varavg_j_)

      std::cout<<entry.first<<" --> "<<entry.second<<"\t";

    std::cout<<std::endl;

    std::cout<<"# err_k_: ";

    for(auto entry: err_k_)

      std::cout<<entry.first<<" --> "<<entry.second<<"\t";

    std::cout<<std::endl;

    std::cout<<"#--other_vars_--"<<std::endl;

    for (auto entry: other_vars_)

      std::cout<<"#\t"<<entry.first<<" is type "<<std::get<0>(other_vars_[entry.first])

      <<": "<<std::get<1>(other_vars_[entry.first])

      <<"\t"<<std::get<2>(other_vars_[entry.first])<<"\t"

      <<std::get<3>(other_vars_[entry.first])<<std::endl;

    std::cout<<"#--input_column_number_--"<<std::endl;

    for (auto entry: input_column_number_) {

      std::vector<int> store=input_column_number_[entry.first];

      std::cout<<"#\t"<<entry.first<<" --> ";

      for(auto val: store){

        std::cout<<val<<"\t";

        std::cout<<std::endl;

      }

    }

  }
  //Sep 2023
  void Data::SetBehavior(){

    using hso::read_data::ColumnType;

    for(auto name: var_bin_names_)

      var_type_[name]=static_cast<int>(ColumnType::VariableBin);

    for(auto name: var_int_names_)

      var_type_[name]=static_cast<int>(ColumnType::VariableRange);

    for(auto name: var_avg_names_)

      var_type_[name]=static_cast<int>(ColumnType::VariableAvg);

    for(auto entry: other_vars_ ) {

      std::string name = entry.first;

      int behavior= std::get<0>(other_vars_[name]);

      var_type_[name]=behavior;

    }

  }

}//hso
