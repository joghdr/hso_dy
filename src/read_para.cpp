#include "read_para.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

namespace hso{

  namespace read_parameters{

    bool verbose=false;

    enum class InputMode {

      Value = 1,
      ValueFree = 2,
      ValueFreeError = 3,
      Full = 6

    };

    int SplitLine(std::string line,std::vector<std::string> &line_split){

      int counter=0;

      std::vector<std::string> (0).swap(line_split);

      std::stringstream ss(line);

      std::string entry("");

      while(ss>>entry){

        line_split.push_back(entry);

        counter++;

      }

      return counter;

    }
    //if there is more than one ocurrence of 'para_name' in 'fname', finds the first one and ignore the rest
    bool FindParameterLine(std::string para_name, std::string fname, std::vector<std::string> &line_split){

      bool found=false;

      std::ifstream file(fname);

      std::string line;

      if(file.good()){

        while(!found&&getline(file,line)){

          int counter=SplitLine(line,line_split);

          if(counter>0&&line_split[0].compare(para_name)==0)

            found=true;

        }

      }

      file.close();

      return found;

    }
    //NOTE
    // returns int = to the case number, to be used by ReadParaValues:
    //
    // InputMode::Value (1 input): takes given value for parameter and fixes it
    //
    // InputMode::ValueFree (2 input): takes first value as the central and second
    // to decide if para is free (0=fixed, other=free)
    // InputMode::ValueFreeError (3 input): as case InputMode::ValueFree, but takes
    // additional third value as the error (useful for minuit)
    // InputMode::Full (4 input): as case InputMode::ValueFreeError,
    // but has 3 more values: limited, min and max;
    // (for 4th value 1=limited, other=not limited)
    // other cases trated as if parameter was not found.
    InputMode GetParameterValues(std::string para_name,std::string fname,

                                 std::vector<double> &values){

      std::vector<double>(0).swap(values);

      std::vector<std::string> line_split(0);

      if (FindParameterLine(para_name, fname, line_split)){

        int line_split_size = static_cast<int>(line_split.size());

        for (int i = 1; i < line_split_size; i++) {

          double val=0;

          std::stringstream ss(line_split[i]);

          ss>>val;

          values.push_back(val);

        }

      }

      InputMode mode = static_cast<InputMode>(values.size());

      return mode;

    }

    bool ReadParaValues(std::string para_name,std::string fname,double &central, int &free,

                        double &err,int &limited, double &min, double &max) {

      bool found=false;

      std::vector<double> values(0);

      InputMode mode = GetParameterValues(para_name,fname,values);

      switch (mode) {

        case InputMode::Full:

          central= values[0];

          free = (int)values[1];

          err = values[2];

          limited= (int)values[3];

          min = values[4];

          max = values[5];

          found=true;

          break;

        case InputMode::ValueFreeError:

          central= values[0];

          free = (int)values[1];

          err = values[2];

          limited= 0;

          min = 0;

          max = 0;

          found=true;

          break;

        case InputMode::ValueFree:

          central= values[0];

          free = (int)values[1];

          err = 0;

          limited= 0;

          min = 0;

          max = 0;

          found=true;

          break;

        case InputMode::Value:

          central= values[0];

          free = 0;

          err = 0;

          limited= 0;

          min = 0;

          max = 0;

          found=true;

          break;

        default:

          std::cout<<"Warning: read_parameters::ReadParaValues \nparameter "<<para_name

                   <<" not found. Fixing "<<para_name<<" = "<<666<<std::endl;

          central= 666;

          free = 0;

          err = 0;

          limited= 0;

          min = 0;

          max = 0;

          found=false;

      }

      return found;

    }

  }//read_parameters

}//hso
