#ifndef _READ_PARA_H_
#define _READ_PARA_H_

#include <string>
#include <vector>

namespace hso{

  namespace read_parameters{

    extern bool verbose;

    enum class InputMode: int;

    int SplitLine(std::string line,std::vector<std::string> &line_split);

    bool FindParameterLine(std::string para_name, std::string fname, std::vector<std::string> &line_split);

    InputMode GetParameterValues(std::string para_name,std::string fname,std::vector<double> &values);

    bool ReadParaValues(std::string para_name,std::string fname,double &central, int &free, double &err,int &limited, double &min, double &max);

  }//read_parameters

}//hso

#endif // _READ_PARA_H_
