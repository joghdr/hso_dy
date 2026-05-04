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
