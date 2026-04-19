#include <string> //iwyu request
#include <hso/objects_theory.h> // IWYU pragma: keep
#include <hso/csec_DY_css_hso_integrands_cuba.h>
#include <hso/theory_class.h>

namespace hso{
  //NOTE: last array in constructors dictate how to treat corresponding variable:
  // // // VariableType::Undetermined if variable is integrated and limits are function of other variables
  // // // VariableType::Value if variable is evaluated at avg value. If data file provides only integration range, mean(varmin,varstd::max) is used.
  // // // VariableType::Integration if variable is integrated with fixed limits taken from data file
  // // // VariableType::BinAvg if variable is integrated with fixed limits taken from data file. Average from data file is passed to integrator,
  // // // but 1/bin factor must be implemented in integrand.
  // // // csec_no_rg_improved namespace to calculate cross section with input functions (no RG improvement)
  // // // csec_rg_improved namespace to calculate cross section with RG improvements
  // NOTE does not use RG improvements (input functions instead), hence namespace is csec_no_rg_improved
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

  ////CDF_I, CDF_II, DO_I
  //NOTE Z0 boson full propagator, integrating over Q
  //     use RG improved version, hence namespace is csec_rg_improved
  hso::Theory observable_drell_yan_Z0_1 (csec_rg_improved::integrand_drell_yan_Z0_1,

                                         {"yh","sqrt_s","Q","qT"},

                                         {VariableType::Undetermined, VariableType::Value,
                                           VariableType::Integration, VariableType::Value}
  );

}//hso
