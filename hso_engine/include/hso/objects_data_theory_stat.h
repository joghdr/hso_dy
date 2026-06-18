#ifndef _OBJECTS_DATA_H_
#define _OBJECTS_DATA_H_
#include <hso/theory_class.h>
#include <string>
#include <vector>
#include <memory>

namespace hso {

  class Data;

  class Experiment;

  class Stat;

  class StatNMin;



}

namespace hso{

  inline constexpr std::string kDirData {"assets/data/"};

  //data
  extern std::unique_ptr<hso::Experiment>  e288_experiment ;
  extern std::unique_ptr<hso::Experiment>  e605_experiment ;

  extern std::vector<hso::Data*> e288;
  extern std::vector<hso::Data*> e605;

  //theory
  extern hso::Theory observable_drell_yan_1;
  extern hso::Theory observable_drell_yan_4_xFavg_qTavg;

  //stat
  extern hso::StatNMin chi2_e288;
  extern hso::StatNMin chi2_e605;

  extern std::vector<hso::Stat*> chi2_total;

}//hso

#endif //_OBJECTS_DATA_H_
