#include "objects_stat.h"
#include "FCN.h"
#include "behavior.h"
#include "utilities.h"
#include "Minuit2/MnUserParameters.h"
#include "Minuit2/FunctionMinimum.h"
#include <string>
#include <iostream>


int main(int argc, char *argv[]){

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

  ROOT::Minuit2::FunctionMinimum *min = hso::Minimize(theFCN,upar);

  hso::Write(hso::chi2_total);//files with data and theory in 'stat' directory

  hso::Plot(hso::chi2_total ,"qT","csec");//writes gnuplot scripts, needed for bash command 'plotE288' and 'plotE288-band'

  hso::Errors(theFCN, *min);//use minuit's covariance matrix to get eigenvalues, writes: <MAIN>/cov/scan.dat <MAIN>/min/eigensets.dat

  std::cout<<"END OF PROGRAM"<<std::endl;

  return 0;

}
