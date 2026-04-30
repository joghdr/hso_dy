#include <hso/objects_stat.h>
#include <hso/FCN.h>
#include <hso/utilities.h>
#include "Minuit2/MnUserParameters.h"
#include <string>
#include <iostream>
#include <vector>

namespace ROOT { namespace Minuit2 { class FunctionMinimum; } }


// #include <iostream>
#include <cstdlib>
// #include <string>

void CheckEnvironment() {
  // Helper to get env or return a "fallback" string
  auto get_env_safe = [](const char* var) -> std::string {
    char* val = std::getenv(var);
    return (val == nullptr) ? "NOT DEFINED (Using internal default)" : std::string(val);
  };

  std::cout << "\n--- Environment  ---" << std::endl;
  std::cout << "CUBACORES:     " << get_env_safe("CUBACORES") << std::endl;
  std::cout << "---------------------------------\n" << std::endl;
}


int main(int argc, char *argv[]){

  CheckEnvironment();

  hso::InitProgram (argc, argv);

  hso::InitCollinear ();

  hso::InitKinematics();
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO: document use of these functions
  // hso::collinear::SetCoefficientsMode(hso::collinear::GetCoefficients::FromGrids);
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  extern ROOT::Minuit2::FCN theFCN;//defined in model file

  ROOT::Minuit2::MnUserParameters upar;

  hso::SetMinuitParameters(upar,theFCN.para_names_);

  int num_of_free_para = hso::utils::GetFreeParaNumber(upar);

  ROOT::Minuit2::FunctionMinimum *min = hso::Minimize(theFCN,upar);

  hso::Write(hso::chi2_total);//files with data and theory in 'stat' directory

  if (num_of_free_para > 0){

    hso::Errors(theFCN, *min);//use minuit's covariance matrix to get eigenvalues, writes: <MAIN>/cov/scan.dat <MAIN>/min/eigensets.dat

  }

  else{

    std::cout<<"All parameters are fixed. No errors to report."<<std::endl;

  }


  std::cout<<"END OF PROGRAM"<<std::endl;

  return 0;

}
