#include <hso/read_kin.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip> //std::setprecision

namespace hso{

  namespace read_kinematics{

    bool verbose=false;

    void CleanLine(std::string &line){

      std::string white(" ");

      std::string::size_type i=line.find(white);

      while(i!=std::string::npos){//clean line to have '#var=value'

        line.erase(i,white.length());

        i=line.find(white);

      }

    }

    std::string GetVariableLine(std::string fname,std::string var){

      std::ifstream file(fname);

      std::string line("");

      std::string output("");

      bool found=false;

      std::string::size_type length=var.length();

      while((!found)&&getline(file,line)){

        CleanLine(line);

        std::string::size_type pos=line.find(var);

        bool format_is_correct=true;

        format_is_correct = format_is_correct&&(pos!=std::string::npos);

        format_is_correct = format_is_correct&&(pos==0);

        format_is_correct = format_is_correct&&(line.compare(pos+length,1,"=")==0);

        if(format_is_correct){

          output=line;

          found=true;

        }

        else {

          output.erase();

        }

      }

      return output;

    }

    double GetLineValue(std::string CleanLine){

      double output;

      std::string::size_type pos = CleanLine.find("=");

      CleanLine.erase(0,pos+1);

      output=stod(CleanLine);

      return output;

    }

    std::string GetLineValue_string(std::string CleanLine){

      std::string output;

      std::string::size_type pos= CleanLine.find("=");

      CleanLine.erase(0,pos+1);

      output=CleanLine;

      return output;

    }

    void ReadKinematics(std::string fname,std::vector<std::string> varnames,void *kin) {

      int varnames_size = static_cast<int>(varnames.size());

      for(int i=0; i < varnames_size; i++){

        std::string var(varnames[i]);

        std::string line;

        double val;

        line=GetVariableLine(fname,var);

        val=GetLineValue(line);

        if(verbose)

          std::cout<<"#reading "<<var<<": "<<std::scientific

                   <<std::setprecision(3)<<val<<std::endl;

        double *pointer_to_var=static_cast<double**>(kin)[i];

        *pointer_to_var=val;

      }

    }

    void ReadString(std::string fname,std::string var,std::string &string_val){

      std::string line=GetVariableLine(fname,var);

      if(verbose)

        std::cout<<"#reading "<<var<<": "<<std::scientific

                 <<std::setprecision(3)<<string_val<<std::endl;

      string_val=GetLineValue_string(line);

    }

  }//read_kinematics

}//hso
