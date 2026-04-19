#include <hso/FCN.h>
#include <hso/pqcd.h>
#include <hso/math_functions.h>
#include <hso/objects_stat.h>
#include <cmath>
#include <string>
#include <vector>
#include <stddef.h>

// currently using serial Minuit2
namespace hso{

  namespace utils {

    extern double Q0;

    extern double rg_transf_Qmax;

    extern double mu_over_Q0;

    extern double sqrt_zeta_over_Q0;

    extern double mu_over_Q;

    extern double sqrt_zeta_over_Q;

  }

  //NOTE: Model definition
  //      CS kernel with QCD constraints
  double KtildeInputOrder_1(double muQ0, double bT, void *Ktildepara){//CS kernel

    double output=0.0;
    //NOTE: the indices here determine the order of the parameters,
    //      the same order should be used in variable 'Ktildepara'
    //      inside 'FCN::operator()' below
    double mk =*static_cast<double**>(Ktildepara)[0];//index = 0 => first parameter

    double bk =*static_cast<double**>(Ktildepara)[1];//index = 1 => second parameter

    output = BesselK0(mk*bT)+std::log(mk/muQ0);

    output *= 2.0 *AlphaStrong(muQ0)*CF/Pi;

    output += ( std::exp(-std::pow(bT*mk,2)) - 1.0 )*bk;

    return output;

  }
  //NOTE: Model definition.
  //      TMDPDF 'core' model without QCD constraints
  //      (constraints already implemented elsewhere)
  double FtildeCore(double x, double bT, void *Ftildepara){//TMD pdf

    double output=0.0;
    //NOTE: the indices here determine the order of the parameters,
    //      the same order should be used in variables 'Ftildepara_a'
    //      and 'Ftildepara_b', inside 'FCN::operator()' below
    //      index = 0 is assigned to the parameter 'mF', already in use
    //      elsewhere in the code, so parameters here start from index=1
    double MF = *static_cast<double**>(Ftildepara)[1]; //index = 1 => second parameter

    double MFlog = *static_cast<double**>(Ftildepara)[2]; //index = 2 => third parameter

    double mass = MF + MFlog * std::log( 1.0/x );

    output=exp(-std::pow(bT*mass,2)/4.0);

    return output;

  }

}//hso

//NOTE:    parameter names should be in the same order in which
//         they are used  by FCN::operator().
//         First entry in 'parameter_names' should always be "mF"
std::vector<std::string> parameter_names {"mF", "MF", "MFlog", "mK", "bK"};

ROOT::Minuit2::FCN theFCN (hso::chi2_total, parameter_names);

double ROOT::Minuit2::FCN::operator()(const std::vector<double>& parameters) const{

  int npts = 0;

  double chi2_value=0;
  //NOTE: mF should always be initialized here
  //      parameters in both functions 'FtildeCore'
  //      and 'KtildeInputOrder_1, defined above,
  //      should be initialized here
  double mF = 0;

  double MF = 0;

  double MFlog = 0;

  double mK = 0;

  double bK = 0;

  using namespace hso::utils;

  void* scales[] = {
    &Q0,
    &rg_transf_Qmax,
    &mu_over_Q0,
    &sqrt_zeta_over_Q0,
    &mu_over_Q,
    &sqrt_zeta_over_Q
  };
  //NOTE first element of 'Ftildepara_a' should always be &mF
  //     since this parameter is already required elsewhere in the code.
  //     The order of remaining parameters should be the same as in
  //     function 'FtildeCore' above
  void* Ftildepara_a[] = {
    &mF,
    &MF,
    &MFlog
  };
  //NOTE: first element of 'Ftildepara_b' should always be &mF
  //      since this parameter is already required elsewhere in the code.
  //      The order of remaining parameters should be the same as in
  //      function 'FtildeCore' above
  void* Ftildepara_b[] = {
    &mF,
    &MF,
    &MFlog
  };
  //NOTE: The order of parameters in 'Ktildepara' should be the same as in
  //      function 'KtildeInputOrder_1' above
  void* Ktildepara[] = {
    &mK,
    &bK
  };

  //NOTE: The indexing here should be consistent with the order
  //      in the variable 'parameter_names' defined above
  //      the first parameter should always be mF
  mF = parameters[0];//first parameter set to mF

  MF = parameters[1];

  MFlog = parameters[2];

  mK = parameters[3];

  bK = parameters[4];
  ///NOTE:    remaining code would usually need no modification
  void *para[]={scales,Ftildepara_a,Ftildepara_b,Ktildepara};

  EvalChi2(para, chi2_value, npts, NULL);

  ROOT::Minuit2::fit_iteration_counter++;

  PrintToScreen(npts, chi2_value, parameters);

  WriteToLog (npts, chi2_value, parameters);

  return chi2_value;

}
