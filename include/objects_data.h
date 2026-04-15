#ifndef _OBJECTS_DATA_H_
#define _OBJECTS_DATA_H_
#include <string>
#include <vector>
#include "data_class.h"

namespace hso{

  inline constexpr std::string kDirData {"./data/"};
  ///@fix_target
  //e288
  extern hso::Data e288_200_4 ;
  extern hso::Data e288_200_5 ;
  extern hso::Data e288_200_6 ;
  extern hso::Data e288_200_7 ;
  extern hso::Data e288_200_8 ;
  extern hso::Data e288_200_9 ;
  extern hso::Data e288_200_10;
  extern hso::Data e288_300_4 ;
  extern hso::Data e288_300_5 ;
  extern hso::Data e288_300_6 ;
  extern hso::Data e288_300_7 ;
  extern hso::Data e288_300_8 ;
  extern hso::Data e288_300_9 ;
  extern hso::Data e288_300_10;
  extern hso::Data e288_300_11;
  extern hso::Data e288_400_5 ;
  extern hso::Data e288_400_6 ;
  extern hso::Data e288_400_7 ;
  extern hso::Data e288_400_8 ;
  extern hso::Data e288_400_9 ;
  extern hso::Data e288_400_10;
  extern hso::Data e288_400_11;
  extern hso::Data e288_400_12;
  extern hso::Data e288_400_13;
  ////e605
  extern hso::Data e605_800_7 ;
  extern hso::Data e605_800_8 ;
  extern hso::Data e605_800_10p5 ;
  extern hso::Data e605_800_11p5 ;
  extern hso::Data e605_800_13p5 ;
  ///@TEVATRON
  ////CDF_I ZPT 1991 Z0 --> e+ e- + Z0 --> mu+ mu- (avg, divided by branching ratio 0.033)
  extern hso::Data cdfI_Z0pt_1991_raw;
  ///@cdf_I_1999_z0-->ee
  extern hso::Data cdfI_Z0pt_1999_raw;
  ///@cdf_I_1999_z0-->ee
  extern hso::Data d0I_Z0pt_1999_raw;
  ///@cdf_I_2012_z0-->ee
  extern hso::Data cdfII_Z0pt_2012_raw;
  //pointers stored in vectors, used by hso::Stat
  extern std::vector<hso::Data*> e288;
  extern std::vector<hso::Data*> e605;
  extern std::vector<hso::Data*> cdfI_Z0pt_1999;
  extern std::vector<hso::Data*> cdfII_Z0pt_2012;
  extern std::vector<hso::Data*> d0I_Z0pt_1999;
  extern std::vector<hso::Data*> tevatron_I_Z0;
  extern std::vector<hso::Data*> tevatron_II_Z0;

}//hso

#endif //_OBJECTS_DATA_H_
