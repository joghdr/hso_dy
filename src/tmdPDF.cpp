#include <hso/tmdPDF.h>
#include <hso/math_functions.h>
#include <hso/pqcd.h>
#include <hso/CSkernel.h>
#include <hso/collinear_factorization.h>
#include <hso/scale_setting.h>
#include <cmath>


namespace hso{
  ///@global
  extern double FtildeCore(double x, double bT, void *Ftildepara);
  ///@tmd_input_bT
  double FtildeInputOrder_1_msbar(double x, double bT,double muQ0,double Q0,

                                  Parton flavor,void *Ftildepara){

    double output=0.0;

    double m=*static_cast<double**>(Ftildepara)[0];

    double Apdfval = hso::collinear::ApdfQuarkQuark(muQ0,x,flavor);

    double Apdf_gval = hso::collinear::ApdfQuarkGluon(muQ0,x,flavor);

    double Bpdfval = hso::collinear::Bpdf(muQ0,x,flavor);

    double cval = hso::collinear::Cpdf(muQ0,Q0,x,flavor,m);

    output = BesselK0(bT*m)*(Apdfval+Apdf_gval+Bpdfval*std::log(bT/C1/m*Q0*Q0));

    output += cval*FtildeCore(x, bT, Ftildepara);

    return output;

  }
  ///@tmd_hso_bT at input scale, uses same ratio muQ0/Q0 for muQ0b/Q0b
  double FtildeUnderlineOrder_1_msbar( double x, double bT,double muQ0,double Q0,

                                       Parton flavor,

                                       void *Ftildepara,void *Ktildepara,

                                       double rg_transf_Qmax){

    double output=0.0;

    if(rg_transf_Qmax<=0.0)

      rg_transf_Qmax=Q0;

    double Q0b=RGScaleTransformation(Q0,rg_transf_Qmax,bT);

    double muQ0b=(muQ0/Q0) * Q0b;

    double Gammaval=Gamma(muQ0b,muQ0);

    double ktildeval=KtildeUnderlineOrder_1(muQ0b, bT,muQ0b, Ktildepara);

    double Ftildeval=FtildeInputOrder_1_msbar(x, bT,muQ0b,Q0b,flavor,Ftildepara);

    output = Ftildeval;

    output *= pow(Q0/Q0b,ktildeval);

    output *= exp(Gammaval);

    return output;

  }

}//hso
