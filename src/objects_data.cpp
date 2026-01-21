#include "objects_data.h"
#include "data_class.h"
#include <vector>
#include <string>

namespace hso{
  ////////////////////////////////////////////////////////////////////
  //NOTE: E288 experiment                                           //
  ////////////////////////////////////////////////////////////////////
  hso::Data e288_200_4 ( kDirData + "/E288/" + "E288_200_4-5.dat" );
  hso::Data e288_200_5 ( kDirData + "/E288/" + "E288_200_5-6.dat" );
  hso::Data e288_200_6 ( kDirData + "/E288/" + "E288_200_6-7.dat" );
  hso::Data e288_200_7 ( kDirData + "/E288/" + "E288_200_7-8.dat" );
  hso::Data e288_200_8 ( kDirData + "/E288/" + "E288_200_8-9.dat" );
  hso::Data e288_200_9 ( kDirData + "/E288/" + "E288_200_9-10.dat" );
  hso::Data e288_200_10( kDirData + "/E288/" + "E288_200_10-11.dat");
  hso::Data e288_300_4 ( kDirData + "/E288/" + "E288_300_4-5.dat" );
  hso::Data e288_300_5 ( kDirData + "/E288/" + "E288_300_5-6.dat" );
  hso::Data e288_300_6 ( kDirData + "/E288/" + "E288_300_6-7.dat" );
  hso::Data e288_300_7 ( kDirData + "/E288/" + "E288_300_7-8.dat" );
  hso::Data e288_300_8 ( kDirData + "/E288/" + "E288_300_8-9.dat" );
  hso::Data e288_300_9 ( kDirData + "/E288/" + "E288_300_9-10.dat" );
  hso::Data e288_300_10( kDirData + "/E288/" + "E288_300_10-11.dat");
  hso::Data e288_300_11( kDirData + "/E288/" + "E288_300_11-12.dat");
  hso::Data e288_400_5 ( kDirData + "/E288/" + "E288_400_5-6.dat" );
  hso::Data e288_400_6 ( kDirData + "/E288/" + "E288_400_6-7.dat" );
  hso::Data e288_400_7 ( kDirData + "/E288/" + "E288_400_7-8.dat" );
  hso::Data e288_400_8 ( kDirData + "/E288/" + "E288_400_8-9.dat" );
  hso::Data e288_400_9 ( kDirData + "/E288/" + "E288_400_9-10.dat" );
  hso::Data e288_400_10( kDirData + "/E288/" + "E288_400_10-11.dat");
  hso::Data e288_400_11( kDirData + "/E288/" + "E288_400_11-12.dat");
  hso::Data e288_400_12( kDirData + "/E288/" + "E288_400_12-13.dat");
  hso::Data e288_400_13( kDirData + "/E288/" + "E288_400_13-14.dat");

  std::vector<hso::Data*> e288 {
    &e288_200_4 ,
    &e288_200_5 ,
    &e288_200_6 ,
    &e288_200_7 ,
    &e288_200_8 ,
    &e288_200_9 ,
    &e288_200_10,
    &e288_300_4 ,
    &e288_300_5 ,
    &e288_300_6 ,
    &e288_300_7 ,
    &e288_300_8 ,
    &e288_300_9 ,
    &e288_300_10,
    &e288_300_11,
    &e288_400_5 ,
    &e288_400_6 ,
    &e288_400_7 ,
    &e288_400_8 ,
    &e288_400_9 ,
    &e288_400_10,
    &e288_400_11,
    &e288_400_12,
    &e288_400_13
  };
  ////////////////////////////////////////////////////////////////////
  //NOTE: E605 experiment                                           //
  ////////////////////////////////////////////////////////////////////
  hso::Data e605_800_7 ( kDirData + "/E605/" + "E605_800_7-8.dat" );
  hso::Data e605_800_8 ( kDirData + "/E605/" + "E605_800_8-9.dat" );
  hso::Data e605_800_10p5 ( kDirData + "/E605/" + "E605_800_10.5-11.5.dat");
  hso::Data e605_800_11p5 ( kDirData + "/E605/" + "E605_800_11.5-13.5.dat");
  hso::Data e605_800_13p5 ( kDirData + "/E605/" + "E605_800_13.5-18.0.dat");

  std::vector<hso::Data*> e605 {
    & e605_800_7,
    & e605_800_8,
    & e605_800_10p5,
    & e605_800_11p5,
    & e605_800_13p5
  };
  ////////////////////////////////////////////////////////////////////
  //NOTE: Tevatron data                                             //
  ////////////////////////////////////////////////////////////////////
  hso::Data cdfI_Z0pt_1991_raw ( kDirData + "/CDFI_ZPT_1991/" + "CDFIZPT_1800_1991.dat" );    //z0-->ee (avg divided by 0.033; superseded by 1999 analysis)
  hso::Data cdfI_Z0pt_1999_raw ( kDirData + "/CDFI_ZPT_1999/" + "CDFIZPT_1800_1999.dat" );    //z0-->ee
  hso::Data d0I_Z0pt_1999_raw ( kDirData + "/D0I_ZPT_1999/" + "D0IZPT_1800_1999.dat" );       //z0-->ee
  hso::Data cdfII_Z0pt_2012_raw ( kDirData + "/CDFII_ZPT_2012/" + "CDFIIZPT_1960_2012.dat" ); //z0-->ee
  ////std::vector wrapper needed for hso::Stat objects
  std::vector<hso::Data*> cdfI_Z0pt_1999 {&cdfI_Z0pt_1999_raw};
  std::vector<hso::Data*> cdfII_Z0pt_2012 {&cdfII_Z0pt_2012_raw};
  std::vector<hso::Data*> d0I_Z0pt_1999 {&d0I_Z0pt_1999_raw};
  std::vector<hso::Data*> tevatron_I_Z0 {&cdfI_Z0pt_1999_raw, &d0I_Z0pt_1999_raw};
  std::vector<hso::Data*> tevatron_II_Z0 {&cdfII_Z0pt_2012_raw};

}//hso
