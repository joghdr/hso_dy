#ifndef _OBJECTS_DATA_H_
#define _OBJECTS_DATA_H_
#include <string>
#include <vector>
#include <memory>

namespace hso {

  class Data;

  class Experiment;

}

namespace hso{

  inline constexpr std::string kDirData {"assets/data/"};
  ///@fix_target
  //e288
  extern std::unique_ptr<hso::Experiment>  e288_experiment ;
  extern std::vector<hso::Data*> e288;

  ////e605
  extern std::unique_ptr<hso::Experiment>  e605_experiment ;
  extern std::vector<hso::Data*> e605;



}//hso

#endif //_OBJECTS_DATA_H_
