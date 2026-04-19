#ifndef _CSKERNEL_H_
#define _CSKERNEL_H_
#include <cstddef> //NULL

namespace hso{

  double KtildeInputOrder_1(double muQ0, double bT, void *Ktildepara);
  //anomalous dimension integrals
  double IntegrateGamma(double (*integrand)( double , void *), void *gammavars);
  //WARNING:log(mup_max/mup)->log(sqrt(zeta_max)/mup), if mup_max=C2 sqrt(zeta_max) with C2=1, its ok
  double GammaIntegrand(double mup,void *gammavars);

  double Gamma(double mup_min,double mup_max);
  //NOTE: to evolve K_tilde_1
  double IntegrateGammaK(double (*integrand)( double ,void*), double mu0, double mu);

  double gamma_K_1(double mup,void *para=NULL);

  double Gamma_K_1(double mu0, double mu);
  //NOTE bT space
  double KtildePertOrder_1(double mu, double bT);

  double KtildePertOrder_1_RG(double mu0,double mu, double bT);

  double KtildeUnderlineOrder_1(double mu, double bT,double muRGScaleTransformation, void *Ktildepara);

}//hso

#endif//_CSKERNEL_H_
