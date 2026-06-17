#ifndef _OBJECTS_STAT_H_
#define _OBJECTS_STAT_H_

#include <vector>

namespace hso{

  class Stat;
  class StatNMin;

}

namespace hso{

  extern hso::StatNMin chi2_e288;
  extern hso::StatNMin chi2_e605;

  extern std::vector<hso::Stat*> chi2_total;





}//hso

#endif //_OBJECTS_STAT_H_
