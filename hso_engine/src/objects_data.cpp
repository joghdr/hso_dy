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
  std::unique_ptr<hso::Experiment> e605_experiment = ExperimentLoader::Load(kDirData + "/E605");

  std::vector<hso::Data*> e605 = e605_experiment->GetDataPointers();


}//hso
