#include "evo.h"
#include "CSkernel.h"
#include "scale_setting.h"
#include <cmath>

namespace hso{

  double TMDEvolutionNoRG(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara){

    double output=0.0;

    double Ktildeval=KtildeInputOrder_1(muQ0, bT, Ktildepara);

    double Gammaval=Gamma(muQ0,muQ);

    output = std::exp(Gammaval);

    output *= std::pow(Q/Q0,Ktildeval);

    return output;

  }

  double WTermEvolutionNoRG(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara){

    double output=0.0;

    output = TMDEvolutionNoRG(bT,muQ0, Q0, muQ, Q,Ktildepara);

    return output*output;

  }

  double TMDEvolution(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara, double rg_transf_Qmax){

    double output=0.0;

    if(rg_transf_Qmax<=0.0)

      rg_transf_Qmax=Q0;

    double Q0b=RGScaleTransformation(Q0,rg_transf_Qmax,bT);

    double muQ0b = (muQ0/Q0) * Q0b;//use same ratio for muQ0/Q0 and muQ0b/Q0b

    double Ktildeval=KtildeUnderlineOrder_1(muQ0, bT,muQ0b, Ktildepara);

    double Gammaval=Gamma(muQ0,muQ);

    output = std::exp(Gammaval);

    output *= std::pow(Q/Q0,Ktildeval);

    return output;

  }

  double WTermEvolution(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara, double rg_transf_Qmax){

    double output=0.0;

    if(rg_transf_Qmax<=0.0)

      rg_transf_Qmax=Q0;

    output = TMDEvolution(bT,muQ0, Q0, muQ, Q,Ktildepara, rg_transf_Qmax);

    return output*output;

  }

}//hso


