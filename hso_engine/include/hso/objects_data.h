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

  ////e605
  extern std::unique_ptr<hso::Data>  e605_800_7 ;
  extern std::unique_ptr<hso::Data>  e605_800_8 ;
  extern std::unique_ptr<hso::Data>  e605_800_10p5 ;
  extern std::unique_ptr<hso::Data>  e605_800_11p5 ;
  extern std::unique_ptr<hso::Data>  e605_800_13p5 ;
  ///@TEVATRON
  ////CDF_I ZPT 1991 Z0 --> e+ e- + Z0 --> mu+ mu- (avg, divided by branching ratio 0.033)
  extern std::unique_ptr<hso::Data>  cdfI_Z0pt_1991_raw;
  ///@cdf_I_1999_z0-->ee
  extern std::unique_ptr<hso::Data>  cdfI_Z0pt_1999_raw;
  ///@cdf_I_1999_z0-->ee
  extern std::unique_ptr<hso::Data>  d0I_Z0pt_1999_raw;
  ///@cdf_I_2012_z0-->ee
  extern std::unique_ptr<hso::Data>  cdfII_Z0pt_2012_raw;
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
