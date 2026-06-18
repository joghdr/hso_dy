#include <hso/objects_data_theory_stat.h>
#include <hso/stat_class.h>
#include <hso/data_class.h>
#include <hso/data_loader.h>
#include <hso/experiment_class.h>
#include <hso/experiment_loader.h>
#include <hso/csec_DY_css_hso_integrands_cuba.h>
#include <vector>
#include <string>
#include <memory>

namespace hso{

  ////////////////////////////////////////////////////////////////////
  //NOTE: Data                                                      //
  ////////////////////////////////////////////////////////////////////
  std::unique_ptr<hso::Experiment> e288_experiment = ExperimentLoader::Load(kDirData + "/E288");
  std::unique_ptr<hso::Experiment> e605_experiment = ExperimentLoader::Load(kDirData + "/E605");

  std::vector<hso::Data*> e288 = e288_experiment->GetDataPointers();
  std::vector<hso::Data*> e605 = e605_experiment->GetDataPointers();

  ////////////////////////////////////////////////////////////////////
  //NOTE: theory                                                    //
  ////////////////////////////////////////////////////////////////////
  //     last array in constructors dictate how to treat corresponding variable:
  // // // VariableType::Undetermined if variable is integrated and limits are function of other variables
  // // // VariableType::Value if variable is evaluated at avg value. If data file provides only integration range, mean(varmin,varstd::max) is used.
  // // // VariableType::Integration if variable is integrated with fixed limits taken from data file
  // // // VariableType::BinAvg if variable is integrated with fixed limits taken from data file. Average from data file is passed to integrator,
  // // // but 1/bin factor must be implemented in integrand.
  // // // csec_no_rg_improved namespace to calculate cross section with input functions (no RG improvement)
  // // // csec_rg_improved namespace to calculate cross section with RG improvements
  //      does not use RG improvements (input functions instead), hence namespace is csec_no_rg_improved
  // if input scale Q0 is lower than 4 GeV, RG improvements are important. To implement them, change namespace below:
  // csec_no_rg_improved -> csec_rg_improved
  ////E288
  hso::Theory observable_drell_yan_1 (csec_no_rg_improved::integrand_drell_yan_1,

                                      {"y","sqrt_s","Q","qT"},

                                      {VariableType::Value, VariableType::Value,
                                        VariableType::Integration, VariableType::Value}

  );

  ////E605
  hso::Theory observable_drell_yan_4_xFavg_qTavg (csec_no_rg_improved::integrand_drell_yan_4_xFavg_qTavg,

                                                  {"xF","sqrt_s","Q","qT"},

                                                  {VariableType::Value, VariableType::Value,
                                                    VariableType::Integration, VariableType::Value}

  );

  ////////////////////////////////////////////////////////////////////
  //NOTE: stat                                                      //
  ////////////////////////////////////////////////////////////////////
  hso::StatNMin chi2_e288 ("E288", &observable_drell_yan_1, e288, 0.00, 0.25);

  hso::StatNMin chi2_e605 ("E605", &observable_drell_yan_4_xFavg_qTavg, e605, 0.05, 0.15);

  std::vector<hso::Stat*> chi2_total { &chi2_e288, &chi2_e605 };


}//hso




namespace hso{



}//hso
