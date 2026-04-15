#include "objects_stat.h"
#include "FCN.h"
#include "utilities.h"
#include "Minuit2/MnUserParameters.h"
#include <iostream>




int main(int argc, char *argv[]){

  hso::InitHessianBands (argc, argv);

  hso::InitCollinear ();

  hso::InitKinematics();
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  //TODO: document use of these functions
  // hso::collinear::SetCoefficientsMode(hso::collinear::GetCoefficients::FromGrids);
  ///////////////////////////////////////////////////////////////////////////////////////////////////
  ROOT::Minuit2::MnUserParameters upar;

  extern ROOT::Minuit2::FCN theFCN;//defined in model file

  hso::SetMinuitParameters(upar,theFCN.para_names_);

  hso::HessianBands(theFCN, upar);

  hso::Write( hso::chi2_total);

  std::cout<<"END OF PROGRAM"<<std::endl;

  return 0;

}
