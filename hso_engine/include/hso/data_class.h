#ifndef _DATA_CLASS_H_
#define _DATA_CLASS_H_

#include <vector>
#include <string>
#include <initializer_list>
#include <map>
#include <tuple>
///@description:
///1) Needs data files prepared in a 'rectangular' grid. Input file should have comments starting with '#'
/// to be ignored. Number of lines and columns are determined automatically. 
/// 
///2) Empty entries in bins, value and error are set to kBlank.
///3) std::vector<bool> Exists determines if there is a data point. Set to false in cases 2)
///4) std::vector<bool> Active to decide which data is gonna be used in a loop, e.g. fit with data cuts
///5) std::vector<double> Data contains data values. One dimensional even for multidimensional data.
///6) std::vector<double> Errors contains errors for a given data point. Errors can be longer than Data if 
/// different types of errors present, e.g. if there are stat and systcorr
// Errors will be twice as long as Data.
namespace hso{

  class Data {

  public:

    //methods
    void UpdateActive();

    std::vector<double> Get(int i,bool check=true,bool omit_errors=false);
    ///read variable values, meas and errors
    bool GetVar(std::string name,int i,std::vector<double> &output);

    void GetVar(std::initializer_list<std::string> keys,int i,std::vector<double> &output);

    void GetErr(std::string name,int i,std::vector<double> &output);

    void GetErr(std::initializer_list<std::string> keys,int i,std::vector<double> &output);

    double GetMeas(int i);

    bool GetVarOther(std::string name,std::vector<double> &output);
    ///@set data ranges
    void CutMin(std::string name,double min,bool action=false);

    void CutMax(std::string name,double max,bool action=false);

    void SetRange(std::string name,double min, double max);
    //Write
    bool Write(std::string file_name_out,bool alldata=true);

    std::vector<double> GetEigenTheory(int eigen_i);

    void Info();

    //members
    bool set_is_active_;

    int length_;

    int length_active_;

    int number_of_eigen_;

    int number_of_var_bin_;

    int number_of_var_int_;

    int number_of_var_avg_;

    int number_of_meas_;

    int number_of_err_;

    std::string name_;

    std::string dir_name_;

    std::string file_name_;

    std::vector<bool> point_is_valid_;

    std::vector<bool> point_is_active_;

    std::vector<std::string> var_bin_names_;

    std::vector<std::string> var_int_names_;

    std::vector<std::string> var_avg_names_;

    std::vector<std::string> meas_names_;

    std::vector<std::string> err_names_;

    std::vector<double> var_bin_avg_;

    std::vector<double> var_bin_min_;

    std::vector<double> var_bin_max_;

    std::vector<double> var_int_min_;

    std::vector<double> var_int_max_;

    std::vector<double> var_avg_;

    std::vector<double> meas_values_;

    std::vector<double> err_values_;

    bool normalize_;

    bool shift_;

    std::vector<double> theory_values_;

    std::vector<double> theory_values_eigen_plus_;//total size = numifeigensets*theory_values.size()/2

    std::vector<double> theory_values_eigen_minus_;//total size = numifeigensets*theory_values.size()/2

    std::vector<double> theory_values_err_plus_;

    std::vector<double> theory_values_err_minus_;

    std::vector<double> norm_values_;

    std::vector<double> shift_values_;

    std::vector<double> stat_values_;

    std::map<std::string,int> varbin_j_;

    std::map<std::string,int> varint_j_;

    std::map<std::string,int> varavg_j_;

    std::map<std::string,int> err_k_;

    std::map<std::string,std::tuple<int,double,double,double>> other_vars_;

    std::map<std::string,std::vector<int>> input_column_number_;

    std::map<std::string,int> var_type_;

  };

}//hso

#endif //_DATA_CLASS_H_
