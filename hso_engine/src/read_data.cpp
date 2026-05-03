#include <hso/read_data.h>
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <utility> //std::pair
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace hso{

  namespace read_data{

    bool verbose=false;
    //mostly for usage by data_class// // // // // // // // // // //
    bool CleanLine(std::string &line, std::string word){

      bool found=false;

      std::string::size_type i=line.find(word);

      found=i!=std::string::npos;//for output

      while(i!=std::string::npos){

        line.erase(i,word.length());

        i=line.find(word);

      }

      return found;

    }

    void SplitLine(std::string line, std::string word,std::vector<std::string> &output){

      std::vector<std::string> ().swap(output);

      std::size_t i=line.find(word);

      while(i!=std::string::npos){

        line.replace(i,word.length()," ");

        i=line.find(word);

      }

      std::stringstream ss(line);

      std::string entry("");

      while(ss >> entry){

        output.push_back(entry);

      }

    }

    bool IsData(std::string line){

      bool output=false;

      CleanLine(line," ");

      output=(line.compare(0,1,kComment)==0)||(line.size()==0);

      return !output;

    }

    bool GetRowCol(std::vector<std::string> &data_lines,std::vector<int> &dim){

      std::vector<int> (2).swap(dim);

      std::vector<int> Columns(0);

      bool is_rectangular=false;

      int number_of_rows =0;

      int number_of_columns=0;

      //get columns line by line
      for (auto line: data_lines){

        int columns=0;

        std::stringstream ss(line);

        std::string entry;

        while(ss>>entry) columns++;

        Columns.push_back(columns);

      }
      //check all Lines have same number of columns
      std::sort(Columns.begin(),Columns.end());

      auto last=unique(Columns.begin(),Columns.end());

      Columns.erase(last,Columns.end());

      is_rectangular=(Columns.size()==1);

      number_of_rows =data_lines.size();

      number_of_columns=Columns[0];

      dim[0]=number_of_rows;

      dim[1]=number_of_columns;

      return is_rectangular;

    }

    void GetLines(std::string fname, std::vector<std::string> &comment_lines, std::vector<std::string> &data_lines, std::vector<int> &dim){

      bool is_rectangular=false;

      std::ifstream Input(fname);

      std::string line_raw;

      if(Input){

        while(getline(Input,line_raw)){

          if(IsData(line_raw)) data_lines.push_back(line_raw);

          else comment_lines.push_back(line_raw);

        }

      }

      else {

        std::cout<<"Error: can't open '"<<fname<<"'. AbortIng."<<std::endl;

        std::exit(0);

      }

      Input.close();

      is_rectangular=GetRowCol(data_lines,dim);

      if(!is_rectangular) {

        std::cout<<"data file '"<<fname<<"' is NOT rectangular. AbortIng."<<std::endl;

        std::exit(1);

      }

    }

    bool GetKey(std::vector<std::string> &comment_lines, std::string &key){

      bool key_found=false;

      std::vector<std::string> Keys(0);

      std::string tag_key(kDataKey);

      for(auto comment: comment_lines){

        std::size_t found;

        std::size_t found2;

        found=comment.find(tag_key);

        if(found!=std::string::npos){

          found2=comment.find(tag_key);

          comment.erase(found2,found2+tag_key.size());

          Keys.push_back(comment);

          key_found=true;}

      }

      if (key_found) key=Keys.back();

      else std::cout<< "#Key not found." <<std::endl;

      if( key_found&&Keys.size()!=1)

        std::cout<< "#Multiple keys found. Using last one." <<std::endl;

      return key_found;

    }

    int GetEntryType(std::string sentry){

      using hso::read_data::ColumnType;

      int type=-1;

      std::vector<std::string> Tags {kTagVariableBin, kTagVariableRange, kTagVariableAvg, kTagMeasurement, kTagError};

      std::map<std::string, int> Types {

        {kTagVariableBin, static_cast<int>(ColumnType::VariableBin)},

        {kTagVariableRange, static_cast<int>(ColumnType::VariableRange)},

        {kTagVariableAvg, static_cast<int>(ColumnType::VariableAvg)},

        {kTagMeasurement, static_cast<int>(ColumnType::Measurement)},

        {kTagError, static_cast<int>(ColumnType::Error)}

      };

      int i=-1;

      std::size_t pos=std::string::npos;

      bool found_type=false;

      int tags_size = static_cast<int>(Tags.size());

      while((!found_type)&&(i != tags_size - 1) ){

        i++;

        pos=sentry.find(Tags[i]);

        if(!(pos==std::string::npos))

          found_type=true;

      }

      if(found_type)

        type=Types[Tags[i]];

      return type;

    }

    bool CheckKey( std::string key,

                   std::vector<std::string> &var_bin_names,

                   std::vector<std::string> &var_int_names) {

      bool key_is_complete=false;

      std::vector<std::string> varbin_subtype {"min","max","avg"};

      std::vector<std::string> varint_subtype {"lower","upper"};
      //check varbin
      bool all_varbin_are_complete=true;

      for(auto name : var_bin_names ) {

        bool varbin_is_complete=true;

        for(auto subtype: varbin_subtype){

          std::stringstream ss(key);

          std::string entry("");

          bool found_subtype=false;

          while(ss>>entry&&(!found_subtype)){

            found_subtype=(entry.compare(kTagVariableBin+name+":"+subtype)==0);

          }

          varbin_is_complete=varbin_is_complete&&found_subtype;

        }

        if(!varbin_is_complete)

          std::cout<<"#"<<__FILE__<<": "<<__LINE__<<": "<<name

                   <<" is Incomplete: tag="<<kTagVariableBin<<std::endl;

        all_varbin_are_complete=all_varbin_are_complete&&varbin_is_complete;

      }

      if(!all_varbin_are_complete)

        std::cout<<"#"<<"VarBin are Incomplete."<<std::endl;

      //check varint
      bool all_varint_are_complete=true;

      for(auto name : var_int_names ) {

        bool varint_is_complete=true;

        for(auto subtype: varint_subtype){

          std::stringstream ss(key);

          std::string entry("");

          bool found_subtype=false;

          while(ss>>entry&&(!found_subtype)){

            found_subtype=(entry.compare(kTagVariableRange+name+":"+subtype)==0);

          }

          varint_is_complete=varint_is_complete&&found_subtype;

        }

        if(!varint_is_complete)

          std::cout<<"#"<<__FILE__<<": "<<__LINE__<<name

                   <<" is Incomplete: tag="<<kTagVariableRange<<std::endl;

        all_varint_are_complete=all_varint_are_complete&&varint_is_complete;

      }

      if(!all_varint_are_complete) std::cout<<"#"<<"Varint are Incomplete."<<std::endl;

      key_is_complete=all_varbin_are_complete&&all_varint_are_complete;

      return key_is_complete;

    }

    bool ParseKey( std::string key,

                  std::vector<std::string> &var_bin_names,

                  std::vector<std::string> &var_int_names,

                  std::vector<std::string> &var_avg_names,

                  std::vector<std::string> &meas_names,

                  std::vector<std::string> &err_names,

                  std::map<std::string,std::vector<int> >&input_column_number){

      bool key_is_complete=false;

      std::vector<std::string> (0).swap(var_bin_names);

      std::vector<std::string> (0).swap(var_int_names);

      std::vector<std::string> (0).swap(var_avg_names);

      std::vector<std::string> (0).swap(meas_names);

      std::vector<std::string> (0).swap(err_names);

      std::stringstream ss(key);

      std::string entry("");

      int column=1;

      std::map<std::string,int> temp_map;

      while(ss >> entry){

        auto type=static_cast<ColumnType>(GetEntryType(entry));

        switch(type){

          case ColumnType::VariableBin:

            CleanLine(entry,kTagVariableBin);

            temp_map.insert(std::pair<std::string,int>(entry,column)); column++;

            CleanLine(entry,":avg");

            CleanLine(entry,":min");

            CleanLine(entry,":max");

            var_bin_names.push_back(entry);

            break;

          case ColumnType::VariableRange:

            CleanLine(entry,kTagVariableRange);

            temp_map.insert(std::pair<std::string,int>(entry,column)); column++;

            CleanLine(entry,":lower");

            CleanLine(entry,":upper");

            var_int_names.push_back(entry);

            break;

          case ColumnType::VariableAvg:

            CleanLine(entry,kTagVariableAvg);

            temp_map.insert(std::pair<std::string,int>(entry,column)); column++;

            var_avg_names.push_back(entry);

            break;

          case ColumnType::Measurement:

            CleanLine(entry,kTagMeasurement);

            temp_map.insert(std::pair<std::string,int>(entry,column)); column++;

            meas_names.push_back(entry);

            break;

          case ColumnType::Error:

            CleanLine(entry,kTagError);

            temp_map.insert(std::pair<std::string,int>(entry,column)); column++;

            err_names.push_back(entry);

            break;

        }

      }
      ///delete repeated
      std::vector<std::string>::iterator it;
      //varbin
      it=unique(var_bin_names.begin(),var_bin_names.end());
      var_bin_names.resize(distance(var_bin_names.begin(),it));
      //varint
      it=unique(var_int_names.begin(),var_int_names.end());

      var_int_names.resize(distance(var_int_names.begin(),it));

      key_is_complete=CheckKey(key,var_bin_names,var_int_names);
      //fill In std::map for column number and variable names
      std::string current_var="";

      for(auto var: var_bin_names) {

        std::vector<int> Cols(0);

        Cols.push_back(temp_map[var+":avg"]);

        Cols.push_back(temp_map[var+":min"]);

        Cols.push_back(temp_map[var+":max"]);

        input_column_number.insert(std::pair<std::string,std::vector<int>>(var,Cols) );

      }

      for(auto var: var_int_names) {

        std::vector<int> Cols(0);

        Cols.push_back(temp_map[var+":lower"]);

        Cols.push_back(temp_map[var+":upper"]);

        input_column_number.insert(std::pair<std::string,std::vector<int>>(var,Cols) );

      }

      for(auto var: var_avg_names) {

        std::vector<int> Cols(0);

        Cols.push_back(temp_map[var]);

        input_column_number.insert(std::pair<std::string,std::vector<int>>(var,Cols) );

      }

      for(auto meas: meas_names) {

        std::vector<int> Cols(0);

        Cols.push_back(temp_map[meas]);

        input_column_number.insert(std::pair<std::string,std::vector<int>>(meas,Cols) );

      }

      for(auto err: err_names) {

        std::vector<int> Cols(0);

        Cols.push_back(temp_map[err]);

        input_column_number.insert(std::pair<std::string,std::vector<int>>(err,Cols) );

      }

      return key_is_complete;

    }

    bool CheckSizes( std::string key,

                      std::vector<std::string> &var_bin_names,

                      std::vector<std::string> &var_int_names,

                      std::vector<std::string> &var_avg_names,

                      std::vector<std::string> &meas_names,

                      std::vector<std::string> &err_names,

                      std::vector<int> &dim){

      std::stringstream ss(key);

      std::string entry("");

      int counter=0;

      while(ss>>entry)counter++;

      int var_bin_names_size = static_cast<int>(var_bin_names.size());

      int var_int_names_size = static_cast<int>(var_int_names.size());

      int var_avg_names_size = static_cast<int>(var_avg_names.size());

      int meas_names_size = static_cast<int>(meas_names.size());

      int err_names_size = static_cast<int>(err_names.size());

      bool key_and_vectors_match=(

        counter == 3 * var_bin_names_size + 2 * var_int_names_size +

                   var_avg_names_size + meas_names_size + err_names_size

      );

      bool key_and_cols_match=(counter==dim[1]);

      if(!key_and_vectors_match)

        std::cout<<"#"<<"key and std::vectors do not match:\n key size="<<counter

                 <<"\tdata cols="

                 <<  3*var_bin_names.size()+2*var_int_names.size() +

                     var_avg_names.size()+meas_names.size()+err_names.size()

                 <<"\n\tkey="<<key<<std::endl;

      if(!key_and_cols_match)

        std::cout<<"#"<<"key and columns do not match:\n key size="<<counter

                 <<"\tdata cols="

                 <<  3*var_bin_names.size()+2*var_int_names.size() +

                     var_avg_names.size()+meas_names.size()+err_names.size()

                 <<"\n\tkey="<<key<<std::endl;

      bool sizes_match = key_and_vectors_match&&key_and_cols_match;

      return sizes_match;

    }

    bool MessageCheck(bool key_is_complete,bool sizes_match){

      bool load_complete=false;

      if(verbose){

        if(key_is_complete)

          std::cout<<"#"<<"key is complete."<<std::endl;

        else

          std::cout<<"#"<<"key is not complete."<<std::endl;

        if(sizes_match)

          std::cout<<"#"<<"sizes match."<<std::endl;

        else

          std::cout<<"#"<<"sizes do not match."<<std::endl;

      }

      load_complete=key_is_complete&&sizes_match;

      return load_complete;

    }

    std::string LoadFile(std::string fname,

                        std::vector<std::string> &data_lines,

                        std::vector<std::string> &comment_lines,

                        std::vector< int> &dim,

                        std::vector<std::string> &var_bin_names,

                        std::vector<std::string> &var_int_names,

                        std::vector<std::string> &var_avg_names,

                        std::vector<std::string> &meas_names,

                        std::vector<std::string> &err_names,

                        std::map<std::string,std::vector<int> >&input_column_number

    ){

      bool key_found=false,key_is_complete=false,sizes_match=false,load_complete=false;

      std::string key;

      GetLines(fname,comment_lines,data_lines,dim);

      key_found=GetKey(comment_lines,key);

      while(!load_complete){

        if(!key_found){

          std::cout<<"#key is not usable. Enter values:"<<std::endl;

          int counter=1;

          std::string entry("");

          std::string key_manual("");

          while(counter!=(dim[1]+1)){

            std::cout<<"#Input key for column "<<counter<<std::endl;

            std::cin>>entry;

            key_manual=key_manual+" "+entry;

            counter++;

          }

          key=key_manual;

        }

        if(verbose){

          std::cout<<"#tryIng key:\n"

                   <<"#\t"<<key<<std::endl;

        }

        key_is_complete = ParseKey(key,var_bin_names,var_int_names,var_avg_names,

                                   meas_names,err_names,input_column_number);

        sizes_match = CheckSizes(key,var_bin_names,var_int_names,var_avg_names,

                                 meas_names,err_names,dim);

        load_complete = MessageCheck(key_is_complete,sizes_match);

        key_found = load_complete;

      }



      return key;

    }

    int GetColumn(std::string key, std::string entry){

      std::stringstream ss(key);

      std::string word("");

      bool found_entry=false;

      int counter=0;

      while(ss>>word&&!found_entry){

        found_entry=(word.compare(entry)==0);

        counter++;

      }

      if(!found_entry)

        counter=0;

      return counter;

    }

    std::map<std::string,int> GetDictionary(std::string key,

                                            std::vector<std::string> &var_bin_names,

                                            std::vector<std::string> &var_int_names,

                                            std::vector<std::string> &var_avg_names,

                                            std::vector<std::string> &meas_names,

                                            std::vector<std::string> &err_names){

      std::map<std::string,int> MAP;

      for(auto varbin: var_bin_names){

        std::vector<std::string> varbin_subtype {"min","max","avg"};

        for(auto subtype: varbin_subtype){

          int col=GetColumn(key,kTagVariableBin+varbin+":"+subtype);

          MAP.insert({varbin+":"+subtype,col});

        }

      }

      for(auto varint: var_int_names){

        std::vector<std::string> varint_subtype {"lower","upper"};

        for(auto subtype: varint_subtype){

          int col=GetColumn(key,kTagVariableRange+varint+":"+subtype);

          MAP.insert({varint+":"+subtype,col});

        }

      }

      for(auto varavg: var_avg_names){

        int col=GetColumn(key,kTagVariableAvg+varavg);

        MAP.insert({varavg,col});

      }

      for(auto meas: meas_names){

        int col=GetColumn(key,kTagMeasurement+meas);

        MAP.insert({meas,col});

      }

      for(auto err: err_names){

        int col=GetColumn(key,kTagError+err);

        MAP.insert({err,col});

      }

      return MAP;

    }

    bool ReadLineValues(std::map<std::string,int> dictionary,

                        std::vector<std::string> &data_lines,

                        std::vector<std::string> &var_bin_names,

                        std::vector<std::string> &var_int_names,

                        std::vector<std::string> &var_avg_names,

                        std::vector<std::string> &meas_names,

                        std::vector<std::string> &err_names,

                        std::vector<double> &var_bin_avg,

                        std::vector<double> &VarBinMIn,

                        std::vector<double> &var_bin_max,

                        std::vector<double> &VarintMIn,

                        std::vector<double> &VarintMax,

                        std::vector<double> &var_avg,

                        std::vector<double> &meas_values,

                        std::vector<double> &err_values){

      bool have_Lines_left=(data_lines.size()!=0);

      if(have_Lines_left){

        std::stringstream dataline(data_lines[0]);

        std::string entry("");

        std::vector<double> dataline_values(0);

        while(dataline>>entry){

          if(entry.compare(kBlank)==0) dataline_values.push_back(NAN);

          else dataline_values.push_back(std::stod(entry));

        }
        //get values for varbin
        for(auto var: var_bin_names){

          int col=-1;

          col=dictionary[var+":avg"];

          var_bin_avg.push_back(dataline_values[col-1]);

        }

        for(auto var: var_bin_names){

          int col=-1;

          col=dictionary[var+":min"];

          VarBinMIn.push_back(dataline_values[col-1]);

        }

        for(auto var: var_bin_names){

          int col=-1;

          col=dictionary[var+":max"];

          var_bin_max.push_back(dataline_values[col-1]);

        }
        //get values for varint
        for(auto var: var_int_names){

          int col=-1;

          col=dictionary[var+":lower"];

          VarintMIn.push_back(dataline_values[col-1]);

        }

        for(auto var: var_int_names){

          int col=-1;

          col=dictionary[var+":upper"];

          VarintMax.push_back(dataline_values[col-1]);

        }
        //get values for varavg
        for(auto var: var_avg_names){

          int col=-1;

          col=dictionary[var];

          var_avg.push_back(dataline_values[col-1]);

        }
        //get values for meas
        for(auto meas: meas_names){

          int col=-1;

          col=dictionary[meas];

          meas_values.push_back(dataline_values[col-1]);

        }
        //get values for errors
        for(auto error: err_names){

          int col=-1;

          col=dictionary[error];

          err_values.push_back(dataline_values[col-1]);

        }

        data_lines.erase(data_lines.begin());

      }

      have_Lines_left=(data_lines.size()!=0);

      return have_Lines_left;

    }

    void SearchOtherKeys(

      std::vector<std::string> &comment_lines,

      std::map<std::string,std::tuple<int,double,double,double>> &other_vars){

      std::vector<std::string> var_bin_names_temp(0);

      std::vector<std::string> var_int_names_temp(0);

      std::vector<std::string> var_avg_names_temp(0);

      std::vector<double> var_bin_avg_temp(0);

      std::vector<double> VarBinMIn_temp(0);

      std::vector<double> var_bin_max_temp(0);

      std::vector<double> VarintMIn_temp(0);

      std::vector<double> var_avgMax_temp(0);

      //find other keys
      for(auto comment: comment_lines){

        if(comment.find(kDataKey)==std::string::npos){

          std::stringstream ss(comment);

          std::string entry;

          while(ss >> entry){

            std::size_t pos=entry.find(kTagVariableBin);

            if(pos==0)

              var_bin_names_temp.push_back(entry);

            else {

              pos=entry.find(kTagVariableRange);

              if(pos==0)

                var_int_names_temp.push_back(entry);

              else{

                pos=entry.find(kTagVariableAvg);

                if(pos==0)

                  var_avg_names_temp.push_back(entry);

              }

            }

          }

        }

      }
      //clean other keys
      for(auto &name: var_bin_names_temp)

        CleanLine(name,kTagVariableBin);

      for(auto &name: var_int_names_temp)

        CleanLine(name,kTagVariableRange);

      for(auto &name: var_avg_names_temp)

        CleanLine(name,kTagVariableAvg);

      if(verbose){

        std::cout<<"#found other key In comments::";

        for(auto name: var_bin_names_temp )

          std::cout<<" varbin:\t"<<name<<"\t";

        std::cout<<std::endl;

        std::cout<<"#found other key In comments::";

        for(auto name: var_int_names_temp )

          std::cout<<" varint:\t"<<name<<"\t";

        std::cout<<std::endl;

        std::cout<<"#found other key In comments::";

        for(auto name: var_avg_names_temp )

          std::cout<<" varavg:\t"<<name<<"\t";

        std::cout<<std::endl;

      }
      //other varbin
      for(auto entry: var_bin_names_temp){

        std::vector<std::string> split(0);

        SplitLine(entry,":",split);

        std::tuple<int,double,double,double> tuple_vals {

                                                static_cast<int>(ColumnType::VariableBin),

                                                std::stod(split[1]),std::stod(split[2]),

                                                std::stod(split[3])

                                             };

        other_vars.insert({split[0],tuple_vals});

      }
      //other varint
      for(auto entry: var_int_names_temp){

        std::vector<std::string> split(0);

        SplitLine(entry,":",split);

        std::tuple<int,double,double,double> tuple_vals {

                                                  static_cast<int>(ColumnType::VariableRange),

                                                  std::stod(split[1]),

                                                  std::stod(split[2]), NAN

                                             };

        other_vars.insert({split[0],tuple_vals});

      }
      //other varavg
      for(auto entry: var_avg_names_temp){

        std::vector<std::string> split(0);

        SplitLine(entry,":",split);

        std::tuple<int,double,double,double> tuple_vals {

                                                  static_cast<int>(ColumnType::VariableAvg),

                                                  std::stod(split[1]), NAN, NAN

                                             };

        other_vars.insert({split[0], tuple_vals});

      }
      // return true;
      }
      ///@other
      bool FileExists (const std::string& name) {

        bool exists;

        std::ifstream f(name.c_str());

        exists= f.good();

        f.close();

        return exists;

      }

      std::string ReadFile(std::string file){

        std::ifstream In(file);

        std::string line, output;

        while(getline(In,line)){

          output=output+"\n"+line;

        }

        In.close();

        return output;

      }

      bool Replace(std::string &line, const std::string &s1,const std::string &s2 ){//Replaces only first

        bool found=false;

        size_t l1=s1.length();

        std::string::size_type pos=line.find(s1);

        found=pos!=std::string::npos;//for output

        if(found)line.replace(pos,l1,s2);

        return found;

      }

      bool Replaceall(std::string &line, const std::string s1,const std::string s2 ){//Replaces all

        bool found=false;

        while(Replace(line,s1,s2 )){

          found=true;

        }

        return found;

      }

      std::string ExtractFname(std::string filenamefull){

        std::string name_f(filenamefull);

        std::string fname;

        std::string name_d(filenamefull);

        std::size_t pos=name_f.rfind("/");

        if(pos!=std::string::npos) {

          name_f.erase(0,pos+1);

          name_d.erase(pos+1,name_d.length()-pos);

        }

        else{

          name_d.assign("./");

        }

        fname=name_f;

        return fname;

      }

  }//read_data

}//hso
