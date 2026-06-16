#include <hso/objects_data.h>
#include <hso/data_class.h>
#include <hso/data_loader.h>
#include <vector>
#include <string>
#include <memory>

namespace hso{

  ////////////////////////////////////////////////////////////////////
  //NOTE: E288 experiment                                           //
  ////////////////////////////////////////////////////////////////////
  std::unique_ptr<hso::Data>  e288_200_4  = DataLoader::Load( kDirData + "/E288/" + "E288_200_4-5.dat" );
  std::unique_ptr<hso::Data>  e288_200_5  = DataLoader::Load( kDirData + "/E288/" + "E288_200_5-6.dat" );
  std::unique_ptr<hso::Data>  e288_200_6  = DataLoader::Load( kDirData + "/E288/" + "E288_200_6-7.dat" );
  std::unique_ptr<hso::Data>  e288_200_7  = DataLoader::Load( kDirData + "/E288/" + "E288_200_7-8.dat" );
  std::unique_ptr<hso::Data>  e288_200_8  = DataLoader::Load( kDirData + "/E288/" + "E288_200_8-9.dat" );
  std::unique_ptr<hso::Data>  e288_200_9  = DataLoader::Load( kDirData + "/E288/" + "E288_200_9-10.dat" );
  std::unique_ptr<hso::Data>  e288_200_10 = DataLoader::Load( kDirData + "/E288/" + "E288_200_10-11.dat");
  std::unique_ptr<hso::Data>  e288_300_4  = DataLoader::Load( kDirData + "/E288/" + "E288_300_4-5.dat" );
  std::unique_ptr<hso::Data>  e288_300_5  = DataLoader::Load( kDirData + "/E288/" + "E288_300_5-6.dat" );
  std::unique_ptr<hso::Data>  e288_300_6  = DataLoader::Load( kDirData + "/E288/" + "E288_300_6-7.dat" );
  std::unique_ptr<hso::Data>  e288_300_7  = DataLoader::Load( kDirData + "/E288/" + "E288_300_7-8.dat" );
  std::unique_ptr<hso::Data>  e288_300_8  = DataLoader::Load( kDirData + "/E288/" + "E288_300_8-9.dat" );
  std::unique_ptr<hso::Data>  e288_300_9  = DataLoader::Load( kDirData + "/E288/" + "E288_300_9-10.dat" );
  std::unique_ptr<hso::Data>  e288_300_10 = DataLoader::Load( kDirData + "/E288/" + "E288_300_10-11.dat");
  std::unique_ptr<hso::Data>  e288_300_11 = DataLoader::Load( kDirData + "/E288/" + "E288_300_11-12.dat");
  std::unique_ptr<hso::Data>  e288_400_5  = DataLoader::Load( kDirData + "/E288/" + "E288_400_5-6.dat" );
  std::unique_ptr<hso::Data>  e288_400_6  = DataLoader::Load( kDirData + "/E288/" + "E288_400_6-7.dat" );
  std::unique_ptr<hso::Data>  e288_400_7  = DataLoader::Load( kDirData + "/E288/" + "E288_400_7-8.dat" );
  std::unique_ptr<hso::Data>  e288_400_8  = DataLoader::Load( kDirData + "/E288/" + "E288_400_8-9.dat" );
  std::unique_ptr<hso::Data>  e288_400_9  = DataLoader::Load( kDirData + "/E288/" + "E288_400_9-10.dat" );
  std::unique_ptr<hso::Data>  e288_400_10 = DataLoader::Load( kDirData + "/E288/" + "E288_400_10-11.dat");
  std::unique_ptr<hso::Data>  e288_400_11 = DataLoader::Load( kDirData + "/E288/" + "E288_400_11-12.dat");
  std::unique_ptr<hso::Data>  e288_400_12 = DataLoader::Load( kDirData + "/E288/" + "E288_400_12-13.dat");
  std::unique_ptr<hso::Data>  e288_400_13 = DataLoader::Load( kDirData + "/E288/" + "E288_400_13-14.dat");

  std::vector<hso::Data*> e288 {
    e288_200_4 .get(),
    e288_200_5 .get(),
    e288_200_6 .get(),
    e288_200_7 .get(),
    e288_200_8 .get(),
    e288_200_9 .get(),
    e288_200_10.get(),
    e288_300_4 .get(),
    e288_300_5 .get(),
    e288_300_6 .get(),
    e288_300_7 .get(),
    e288_300_8 .get(),
    e288_300_9 .get(),
    e288_300_10.get(),
    e288_300_11.get(),
    e288_400_5 .get(),
    e288_400_6 .get(),
    e288_400_7 .get(),
    e288_400_8 .get(),
    e288_400_9 .get(),
    e288_400_10.get(),
    e288_400_11.get(),
    e288_400_12.get(),
    e288_400_13.get()
  };
  ////////////////////////////////////////////////////////////////////
  //NOTE: E605 experiment                                           //
  ////////////////////////////////////////////////////////////////////
  std::unique_ptr<hso::Data>  e605_800_7    = DataLoader::Load( kDirData + "/E605/" + "E605_800_7-8.dat" );
  std::unique_ptr<hso::Data>  e605_800_8    = DataLoader::Load( kDirData + "/E605/" + "E605_800_8-9.dat" );
  std::unique_ptr<hso::Data>  e605_800_10p5 = DataLoader::Load( kDirData + "/E605/" + "E605_800_10.5-11.5.dat");
  std::unique_ptr<hso::Data>  e605_800_11p5 = DataLoader::Load( kDirData + "/E605/" + "E605_800_11.5-13.5.dat");
  std::unique_ptr<hso::Data>  e605_800_13p5 = DataLoader::Load( kDirData + "/E605/" + "E605_800_13.5-18.0.dat");

  std::vector<hso::Data*> e605 {
    e605_800_7   .get(),
    e605_800_8   .get(),
    e605_800_10p5.get(),
    e605_800_11p5.get(),
    e605_800_13p5.get()
  };
  ////////////////////////////////////////////////////////////////////
  //NOTE: Tevatron data                                             //
  ////////////////////////////////////////////////////////////////////
  std::unique_ptr<hso::Data>  cdfI_Z0pt_1991_raw  =DataLoader::Load( kDirData + "/CDFI_ZPT_1991/" + "CDFIZPT_1800_1991.dat" );    //z0-->ee (avg divided by 0.033; superseded by 1999 analysis)
  std::unique_ptr<hso::Data>  cdfI_Z0pt_1999_raw  =DataLoader::Load( kDirData + "/CDFI_ZPT_1999/" + "CDFIZPT_1800_1999.dat" );    //z0-->ee
  std::unique_ptr<hso::Data>  d0I_Z0pt_1999_raw   =DataLoader::Load( kDirData + "/D0I_ZPT_1999/" + "D0IZPT_1800_1999.dat" );       //z0-->ee
  std::unique_ptr<hso::Data>  cdfII_Z0pt_2012_raw =DataLoader::Load( kDirData + "/CDFII_ZPT_2012/" + "CDFIIZPT_1960_2012.dat" ); //z0-->ee
  ////std::vector wrapper needed for hso::Stat objects
  std::vector<hso::Data *> cdfI_Z0pt_1999 {cdfI_Z0pt_1999_raw.get()};
  std::vector<hso::Data *> cdfII_Z0pt_2012 {cdfII_Z0pt_2012_raw.get()};
  std::vector<hso::Data *> d0I_Z0pt_1999 {d0I_Z0pt_1999_raw.get()};
  std::vector<hso::Data *> tevatron_I_Z0 {cdfI_Z0pt_1999_raw.get(), d0I_Z0pt_1999_raw.get()};
  std::vector<hso::Data *> tevatron_II_Z0 {cdfII_Z0pt_2012_raw.get()};

}//hso
