#ifndef _READ_DATA_H_
#define _READ_DATA_H_

#include <string>
#include <vector>
#include <map>
#include <tuple>
//data class and read_data.cpp: general use definions
////used only explicitly in read_data.cpp
inline constexpr char kBlank[] = "-";
inline constexpr char kComment[] = "#";
inline constexpr char kDataKey[] = "###key::";
////used explicitly in read_data.cpp
inline constexpr char kTagVariableBin[] = "varbin:";
inline constexpr char kTagVariableRange[] = "varint:";
inline constexpr char kTagVariableAvg[] = "varavg:";
inline constexpr char kTagMeasurement[] = "meas:";
inline constexpr char kTagError[] = "err:";


namespace hso{

  namespace read_data{

    extern bool verbose;

    enum class ColumnType {
      VariableBin = 0,
      VariableRange = 1,
      VariableAvg = 2,
      Measurement = 3,
      Error = 4
    };

    ///@data_class uses these mostly // // // // // // // // // // //
    bool CleanLine(std::string &line, std::string word);

    void SplitLine(std::string line, std::string word,std::vector<std::string> &output);

    bool IsData(std::string line);

    bool GetRowCol(std::vector<std::string> &data_lines,std::vector<int> &dim);

    void GetLines(std::string fname, std::vector<std::string> &comment_lines, std::vector<std::string> &data_lines, std::vector<int> &dim);

    bool GetKey(std::vector<std::string> &comment_lines, std::string &key);

    int GetEntryType(std::string sentry);

    bool CheckKey( std::string key,
                   std::vector<std::string> &var_bin_names,
                   std::vector<std::string> &var_int_names);

    bool ParseKey( std::string key,
                   std::vector<std::string> &var_bin_names,
                   std::vector<std::string> &var_avg_names,
                   std::vector<std::string> &var_int_names,
                   std::vector<std::string> &meas_names,
                   std::vector<std::string> &err_names,
                   std::map<std::string,std::vector<int> >&input_column_number);


    bool CheckSizes( std::string key,
                     std::vector<std::string> &var_bin_names,
                     std::vector<std::string> &var_int_names,
                     std::vector<std::string> &var_avg_names,
                     std::vector<std::string> &meas_names,
                     std::vector<std::string> &err_names,
                     std::vector<int> &dim);

    bool MessageCheck(bool key_is_complete,bool sizes_match);

    std::string LoadFile(std::string fname,
                         std::vector<std::string> &data_lines,
                         std::vector<std::string> &comment_lines,
                         std::vector< int> &dim,
                         std::vector<std::string> &var_bin_names,
                         std::vector<std::string> &var_int_names,
                         std::vector<std::string> &var_avg_names,
                         std::vector<std::string> &meas_names,
                         std::vector<std::string> &err_names,
                         std::map<std::string,std::vector<int> >&input_column_number);

    int GetColumn(std::string key, std::string entry);

    std::map<std::string,int> GetDictionary(std::string key,
                                             std::vector<std::string> &var_bin_names,
                                             std::vector<std::string> &var_int_names,
                                             std::vector<std::string> &var_avg_names,
                                             std::vector<std::string> &meas_names,
                                             std::vector<std::string> &err_names);

    bool ReadLineValues(std::map<std::string,int> dictionary,
                        std::vector<std::string> &data_lines,
                        std::vector<std::string> &var_bin_names,
                        std::vector<std::string> &var_int_names,
                        std::vector<std::string> &var_avg_names,
                        std::vector<std::string> &meas_names,
                        std::vector<std::string> &err_names,
                        std::vector<double> &var_bin_avg,
                        std::vector<double> &var_bin_min,
                        std::vector<double> &var_bin_max,
                        std::vector<double> &var_int_min,
                        std::vector<double> &var_int_max,
                        std::vector<double> &var_avg,
                        std::vector<double> &meas_values,
                        std::vector<double> &err_values);

    void SearchOtherKeys(std::vector<std::string> &comment_lines,
                         std::map<std::string,std::tuple<int,double,double,double>> &other_vars);

    bool FileExists (const std::string& name);

    std::string ReadFile(std::string file);

    bool Replace(std::string &line, const std::string &s1,const std::string &s2 );

    bool Replaceall(std::string &line, const std::string s1,const std::string s2 );

    std::string ExtractFname(std::string filenamefull);

  }//read_data

}//hso

#endif //_READ_DATA_H_
