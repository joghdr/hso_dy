#ifndef _READ_KIN_H_
#define _READ_KIN_H_

#include <string>
#include <vector>

namespace hso{

  namespace read_kinematics{

    extern bool verbose;

    void CleanLine(std::string &line);

    std::string GetVariableLine(std::string fname,std::string var);

    double GetLineValue(std::string CleanLine);

    std::string GetLineValue_string(std::string CleanLine);

    void ReadKinematics(std::string fname,std::vector<std::string> varnames,void *kin);

    void ReadString(std::string fname,std::string var,std::string &string_val);

  }//read_kinematics

}//hso

#endif // _READ_KIN_H_
