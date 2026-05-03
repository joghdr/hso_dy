#include <hso/data_loader.h>
#include <hso/data_class.h>
#include <hso/read_data.h>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <iostream>
#include <stdexcept>//std::out_of_range
#include <initializer_list>
#include <tuple>
#include <fstream>
#include <cstdlib>
#include <cmath>



namespace hso{

  void DataLoader::SetBehavior(Data &data_instance){

    using hso::read_data::ColumnType;

    for(auto name: data_instance.var_bin_names_)

      data_instance.var_type_[name]=static_cast<int>(ColumnType::VariableBin);

    for(auto name: data_instance.var_int_names_)

      data_instance.var_type_[name]=static_cast<int>(ColumnType::VariableRange);

    for(auto name: data_instance.var_avg_names_)

      data_instance.var_type_[name]=static_cast<int>(ColumnType::VariableAvg);

    for(auto entry: data_instance.other_vars_ ) {

      std::string name = entry.first;

      int behavior= std::get<0>(data_instance.other_vars_[name]);

      data_instance.var_type_[name]=behavior;

    }

  }

  Data DataLoader::Load(std::string fname_in){

    std::string name_f(fname_in);

    std::string name_d(fname_in);

    std::size_t pos=name_f.rfind("/");

    Data data_instance;

    if(pos!=std::string::npos) {

      name_f.erase(0,pos+1);

      name_d.erase(pos+1,name_d.length()-pos);

    }

    else {

      name_d.assign("./");

    }

    data_instance.file_name_=name_f;

    data_instance.dir_name_=name_d;

    pos=name_f.rfind(".");

    if (pos != std::string::npos) {

      name_f.erase(pos,name_f.length()-pos);

      data_instance.name_ = name_f;

    }

    else

      data_instance.name_ = name_f;


    std::vector<std::string> comment_lines;

    std::vector<std::string> data_lines;

    std::vector<int> dim(2);

    std::string key = read_data::LoadFile(data_instance.dir_name_ + data_instance.file_name_, data_lines,

                                        comment_lines, dim, data_instance.var_bin_names_,

                                        data_instance.var_int_names_, data_instance.var_avg_names_, data_instance.meas_names_,

                                        data_instance.err_names_, data_instance.input_column_number_);

    std::map<std::string,int> dictionary;

    dictionary=read_data::GetDictionary(key, data_instance.var_bin_names_, data_instance.var_int_names_,

                                        data_instance.var_avg_names_, data_instance.meas_names_, data_instance.err_names_);

    while (read_data::ReadLineValues(dictionary, data_lines, data_instance.var_bin_names_,

      data_instance.var_int_names_,data_instance.var_avg_names_,data_instance.meas_names_,data_instance.err_names_,

      data_instance.var_bin_avg_,data_instance.var_bin_min_,data_instance.var_bin_max_,data_instance.var_int_min_,

      data_instance.var_int_max_,data_instance.var_avg_,data_instance.meas_values_,data_instance.err_values_)
    );

    data_instance.number_of_var_bin_ = data_instance.var_bin_names_.size();

    data_instance.number_of_var_int_ = data_instance.var_int_names_.size();

    data_instance.number_of_var_avg_ = data_instance.var_avg_names_.size();

    data_instance.number_of_meas_ = 1;

    data_instance.number_of_err_=data_instance.err_names_.size();
    //maps for easy acess to values
    for(int j=0;j<data_instance.number_of_var_bin_;j++)

      data_instance.varbin_j_.insert({data_instance.var_bin_names_[j],j});

    for(int j=0;j<data_instance.number_of_var_int_;j++)

      data_instance.varint_j_.insert({data_instance.var_int_names_[j],j});

    for(int j=0;j<data_instance.number_of_var_avg_;j++)

      data_instance.varavg_j_.insert({data_instance.var_avg_names_[j],j});

    for(int k=0;k<data_instance.number_of_err_ ;k++)

      data_instance.err_k_.insert({data_instance.err_names_[k],k});
    //set active length, valid and active
    data_instance.length_ = data_instance.meas_values_.size();
    //mark lines that are not valid
    std::vector<bool> (data_instance.length_,false).swap(data_instance.point_is_valid_);

    for(int i=0;i<data_instance.length_;i++ ) if(data_instance.meas_values_[i]==data_instance.meas_values_[i]) data_instance.point_is_valid_[i]=true;
    //mark all valid bins as active and count active bins
    std::vector<bool> (data_instance.length_,false).swap(data_instance.point_is_active_);

    data_instance.length_active_=0;

    for (int i=0;i<data_instance.length_;i++ ) {

      if(data_instance.point_is_valid_[i]) {

        data_instance.point_is_active_[i]=true;

        data_instance.length_active_++;

        data_instance.set_is_active_=true;

      }

    }

    read_data::SearchOtherKeys(comment_lines, data_instance.other_vars_);
    //Sep 2023
    std::vector<double> (data_instance.meas_values_.size(),NAN).swap(data_instance.theory_values_);

    std::vector<double> (data_instance.meas_values_.size(),1.0).swap(data_instance.norm_values_);

    std::vector<double> (data_instance.meas_values_.size(),0.0).swap(data_instance.shift_values_);

    data_instance.normalize_=false;

    data_instance.shift_=false;

    SetBehavior(data_instance);

    return data_instance;

  }

}//hso
