#ifndef _DATA_LOADER_H_
#define _DATA_LOADER_H_

#include <vector>
#include <string>
#include <initializer_list>
#include <map>
#include <tuple>

namespace hso{

  class Data;

  class DataLoader {

  public:

    static Data Load(std::string file_name_in);

    static std::string LoadFile(std::string fname,
                         std::vector<std::string> &data_lines,
                         std::vector<std::string> &comment_lines,
                         std::vector< int> &dim,
                         std::vector<std::string> &var_bin_names,
                         std::vector<std::string> &var_int_names,
                         std::vector<std::string> &var_avg_names,
                         std::vector<std::string> &meas_names,
                         std::vector<std::string> &err_names,
                         std::map<std::string,std::vector<int> >&input_column_number);

    static std::map<std::string,int> GetDictionary(std::string key,
                                            std::vector<std::string> &var_bin_names,
                                            std::vector<std::string> &var_int_names,
                                            std::vector<std::string> &var_avg_names,
                                            std::vector<std::string> &meas_names,
                                            std::vector<std::string> &err_names);

    static bool ReadLineValues(std::map<std::string,int> dictionary,
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

    static void SearchOtherKeys(std::vector<std::string> &comment_lines,
                         std::map<std::string,std::tuple<int,double,double,double>> &other_vars);

    static void GetLines(std::string fname, std::vector<std::string> &comment_lines, std::vector<std::string> &data_lines, std::vector<int> &dim);//NOTE:move

    static bool GetKey(std::vector<std::string> &comment_lines, std::string &key);//NOTE:move

    static bool CheckKey( std::string key,
                   std::vector<std::string> &var_bin_names,
                   std::vector<std::string> &var_int_names);


    static bool ParseKey( std::string key,
                   std::vector<std::string> &var_bin_names,
                   std::vector<std::string> &var_avg_names,
                   std::vector<std::string> &var_int_names,
                   std::vector<std::string> &meas_names,
                   std::vector<std::string> &err_names,
                   std::map<std::string,std::vector<int> >&input_column_number);//NOTE:move

    static bool CheckSizes( std::string key,//NOTE:move
                     std::vector<std::string> &var_bin_names,
                     std::vector<std::string> &var_int_names,
                     std::vector<std::string> &var_avg_names,
                     std::vector<std::string> &meas_names,
                     std::vector<std::string> &err_names,
                     std::vector<int> &dim);

    static bool MessageCheck(bool key_is_complete,bool sizes_match);//NOTE:move

    static int GetColumn(std::string key, std::string entry);//NOTE:move

    static int GetEntryType(std::string sentry);

    static bool GetRowCol(std::vector<std::string> &data_lines,std::vector<int> &dim);

    static bool IsData(std::string line);

    static void SetBehavior(Data &datastance);


  };

}//hso

#endif //_DATA_LOADER_H_
