#include "CSkernel.h"
#include "gsl_settings.h"
#include "pqcd.h"
#include <gsl/gsl_integration.h>
#include <iostream>

namespace hso{

  //NOTE function must be defined in model file
  double KtildeInputOrder_1(double muQ0, double bT, void *Ktildepara);

  double IntegrateGamma ( double (*integrand)( double , void *), void *gammavars) {

    double output=0.0, error=0.0;

    double mup_min = *static_cast<double**>(gammavars)[0];

    double mup_max = *static_cast<double**>(gammavars)[1];

    using namespace hso::gsl_default;

    gsl_integration_workspace * WGamma = gsl_integration_workspace_alloc (kGSLWIntervals);

    gsl_function FGamma; //anomalous dimensions

    FGamma.function = integrand;

    FGamma.params = gammavars;

    gsl_integration_qag( &FGamma,mup_min, mup_max, kGSLEpsAbs, kGSLEpsRel, kGSLLimit, kGSLKey , WGamma, &output, &error);

    gsl_integration_workspace_free(WGamma);

    return output;

  }
  //WARNING:log(mup_max/mup)->log(sqrt(zeta_max)/mup), if mup_max=C2 std::sqrt(zeta_max) with C2=1, its ok
  double GammaIntegrand(double mup,void *gammavars){

    double output = 0.0;

    double mup_max = *static_cast<double**>(gammavars)[1];

    output = as(mup)*(gammaF1 - gammak1*std::log(mup_max/mup));

    output /= mup;

    return output;

  }

  double Gamma(double mup_min,double mup_max){

    double output = 0.0;

    void *gammavars[]=

    {
      &mup_min,

      &mup_max

    };

    output = IntegrateGamma(GammaIntegrand,gammavars);

    return output;

  }
  //NOTE: to evolve K_tilde_1
  double IntegrateGammaK (double (*integrand)( double ,void*), double mu0, double mu) {

    double output=0.0, error=0.0;

    using namespace hso::gsl_default;

    gsl_integration_workspace * WGammaK = gsl_integration_workspace_alloc (kGSLWIntervals);

    gsl_function FGammaK; //anomalous dimensions K

    FGammaK.function = integrand;

    gsl_integration_qag( &FGammaK,mu0, mu, kGSLEpsAbs, kGSLEpsRel, kGSLLimit, kGSLKey , WGammaK, &output, &error);

    gsl_integration_workspace_free(WGammaK);

    return output;

  }

  double gamma_K_1(double mup,void *para){

    double output = 0.0;

    output = as(mup)*(gammak1);

    output /= mup;

    return output;

  }

  double Gamma_K_1(double mu0, double mu){

    double output = 0.0;

    output = IntegrateGammaK(gamma_K_1,mu0,mu);

    return output;

  }
  //NOTE bT space
  double KtildePertOrder_1(double mu, double bT){//pqcd calculation

    double output=0.0;

    output = -2.0*CF/Pi;

    output *= std::log(bT*mu/C1);

    output *= AlphaStrong(mu);

    return output;

  }

  double KtildePertOrder_1_RG(double mu0,double mu, double bT){//pqcd calculation

    double output=0.0;

    output = KtildePertOrder_1(mu0, bT);

    output -= Gamma_K_1(mu0,mu);

    return output;

  }

  double KtildeUnderlineOrder_1(double mu, double bT,double muRGScaleTransformation, void *Ktildepara){

    double output=0.0;

    output = KtildeInputOrder_1(muRGScaleTransformation, bT, Ktildepara);

    output -= Gamma_K_1(muRGScaleTransformation,mu);

    return output;

  }

}//hso
