#ifndef _OBJECTS_STAT_H_
#define _OBJECTS_STAT_H_

#include <hso/stat_class.h>
#include <vector>

namespace hso{

  extern hso::StatNMin chi2_e288;
  extern hso::StatNMin chi2_e605;
  extern hso::StatNMin chi2_cdf_I;
  extern hso::StatNMin chi2_cdf_II;
  extern hso::StatNMin chi2_d0_I;

  extern std::vector<hso::Stat*> chi2_total;





}//hso

#endif //_OBJECTS_STAT_H_
