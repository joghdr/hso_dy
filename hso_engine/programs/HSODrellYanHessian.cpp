#include <hso/objects_data_theory_stat.h>
#include <hso/FCN.h>
#include <hso/utilities.h>
#include "Minuit2/MnUserParameters.h"
#include <iostream>
#include <vector>



int main(int argc, char *argv[]){

  std::cout<<"STARTING "<<argv[0]<<"\n";
  hso::InitHessianBands (argc, argv);
  hso::InitCollinearHessian();
  hso::InitKinematicsHessian();
  ROOT::Minuit2::MnUserParameters upar;

  extern ROOT::Minuit2::FCN theFCN;//defined in model file

  hso::SetMinuitParameters(upar,theFCN.para_names_,"hessian_output");
  hso::HessianBands(theFCN, upar);
  std::cout<<"STARTING WRITE\n";
  hso::Write(hso::chi2_total,"hessian_output");
  std::cout<<"END OF PROGRAM"<<std::endl;

  return 0;

}
