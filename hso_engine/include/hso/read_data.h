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

    bool FileExists (const std::string& name);

    std::string ReadFile(std::string file);

    bool Replace(std::string &line, const std::string &s1,const std::string &s2 );

    bool Replaceall(std::string &line, const std::string s1,const std::string s2 );

    std::string ExtractFname(std::string filenamefull);

  }//read_data

}//hso

#endif //_READ_DATA_H_
