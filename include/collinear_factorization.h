#ifndef _COLLINEAR_H_
#define _COLLINEAR_H_

#include "pqcd.h"
#include <string>

namespace hso{

  namespace collinear{

    void SetLhapdf(std::string lhapdf_set,int setid);

    double Pdf(double scale, double x, Parton flavor, bool freeze=false);
    //MSbar collinear PDF from LHAPDF
    double conv_A_tmdpdf_qq_order_1_integrand( double xi, void *vars);

    double conv_A_tmdpdf_qq_order_1(void *vars);

    double conv_A_tmdpdf_qg_order_1_integrand( double xi, void *vars);

    double conv_A_tmdpdf_qg_order_1(void *vars);

    double ApdfQuarkQuarkDirect(double muQ0,double x,Parton flavor);

    double ApdfQuarkGluonDirect(double muQ0,double x,Parton flavor);

    double ApdfQuarkQuarGrid(double muQ0, double x, Parton flavor);//used precomputed values from lhapdf grid

    double ApdfQuarkGluonGrid(double muQ0, double x, Parton flavor);//used precomputed values from lhapdf grid

    double ApdfQuarkQuark(double muQ0,double x,Parton flavor);

    double ApdfQuarkGluon(double muQ0,double x,Parton flavor);

    double BpdfDirect(double muQ0,double x,Parton flavor);

    double BpdfGrid(double muQ0,double x,Parton flavor);//used precomputed values from lhapdf grid

    double Bpdf(double muQ0,double x,Parton flavor);

    double Cpdf(double muQ0,double Q0,double x,Parton flavor,double m);

    double TmdPdfPert( double x, double muQ, double Q, double kT, Parton flavor );

  }//collinear

}//hso
#endif // _COLLINEAR_H_
