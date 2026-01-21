#include "FCN.h"
#include "Minuit2/FunctionMinimum.h"
#include "pqcd.h"
#include "math_functions.h"
#include "objects_stat.h"
#include <cmath>

//NOTE: currently program requires using environment variable
//      OMP_NUM_THREADS = 1 (omp critical region not needed)
//      For now, multiple cores (if available) are used by CUBA integration
//      through the environment variable CUBACORES

namespace hso{

  namespace utils {

    extern double Q0;

    extern double rg_transf_Qmax;

    extern double mu_over_Q0;

    extern double sqrt_zeta_over_Q0;

    extern double mu_over_Q;

    extern double sqrt_zeta_over_Q;

  }
  ///////////////////////////////////////////////////////////////////////
  ///NOTE: Model definitions.                                         ///
  /// must recompile for each different model                         ///
  ///////////////////////////////////////////////////////////////////////
  double KtildeInputOrder_1(double muQ0, double bT, void *Ktildepara){

    double output=0.0;

    double mk =*static_cast<double**>(Ktildepara)[0];

    double bk =*static_cast<double**>(Ktildepara)[1];

    output = BesselK0(mk*bT)+std::log(mk/muQ0);

    output *= 2.0 *AlphaStrong(muQ0)*CF/Pi;

    output += ( std::exp(-std::pow(bT*mk,2)) - 1.0 )*bk;

    return output;

  }

  double FtildeCore(double x, double bT, void *Ftildepara){

    double output=0.0;

    double MF =*static_cast<double**>(Ftildepara)[1];

    double MFlog=*static_cast<double**>(Ftildepara)[2];

    double mass2 = std::exp(-MF) + std::exp(-MFlog) * std::pow(log( 1.0/x ), 2.0);

    output=exp(-std::pow(bT,2)*mass2/4.0);

    return output;

  }

}//hso

///////////////////////////////////////////////////////////////////////
///NOTE:    Creating FCN object and defining FCN::operator(),required//
///         by Minuit. See aldo FCN.h and FCN.cpp files              //
///WARNING: parameter names should be in the same order in which     //
///         they are called by FCN::operator() and by models abote.  //
///////////////////////////////////////////////////////////////////////
std::vector<std::string> parameter_names {"mF", "MF", "MFlog", "mK", "bK"};

ROOT::Minuit2::FCN theFCN (hso::chi2_total, parameter_names);

double ROOT::Minuit2::FCN::operator()(const std::vector<double>& Parameters) const{

  int npts =0;

  double chi2_value=0;

  double mF,MF,MFlog;

  double mK,bK;

  using namespace hso::utils;//to access variables inside 'scales'

  void* scales[] = {
    &Q0,
    &rg_transf_Qmax,
    &mu_over_Q0,
    &sqrt_zeta_over_Q0,
    &mu_over_Q,
    &sqrt_zeta_over_Q
  };

  void* Ftilde_a_para[] = {
    &mF,
    &MF,
    &MFlog
  };

  void* Ftilde_b_para[] = {
    &mF,
    &MF,
    &MFlog
  };

  void* Ktildepara[] = {
    &mK,
    &bK
  };
  ///////////////////////////////////////////////////////////////////////
  ///WARNING: parameters in the same order here as in 'parameter_names'//
  ///////////////////////////////////////////////////////////////////////
  mF = Parameters[0];

  MF = Parameters[1];

  MFlog = Parameters[2];

  mK = Parameters[3];

  bK = Parameters[4];
  ///////////////////////////////////////////////////////////////////////
  ///NOTE:    remaining code would usually need no modification        //
  ///////////////////////////////////////////////////////////////////////
  void *para[]={scales,Ftilde_a_para,Ftilde_b_para,Ktildepara};

  EvalChi2(para, chi2_value, npts, NULL);

  ROOT::Minuit2::fit_iteration_counter++;

  PrintToScreen(npts, chi2_value, Parameters);

  WriteToLog (npts, chi2_value, Parameters);

  return chi2_value;

}
