#include <hso/objects_data.h>
#include <hso/data_class.h>
#include <hso/data_loader.h>
#include <hso/experiment_class.h>
#include <hso/experiment_loader.h>
#include <vector>
#include <string>
#include <memory>

namespace hso{

  ////////////////////////////////////////////////////////////////////
  //NOTE: E288 experiment                                           //
  ////////////////////////////////////////////////////////////////////
  std::unique_ptr<hso::Experiment> e288_experiment = ExperimentLoader::Load(kDirData + "/E288");

  std::vector<hso::Data*> e288 = e288_experiment->GetDataPointers();
  
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
