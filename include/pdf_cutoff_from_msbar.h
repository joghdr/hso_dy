#ifndef _PDF_H_
#define _PDF_H_


namespace hso{

  enum class Parton;

  namespace collinear{

    enum class SchemeChoice {MSBar, CutoffFromMSbar};

    double ConvolutionX(double (*integrand)(double, void*), void *vars);

    double conv_Delta_pdf_qq_order_1_integrand( double xi, void *vars);

    double conv_Delta_pdf_qq_order_1(void *vars);

    double conv_Delta_pdf_qg_order_1_integrand( double xi, void *vars);

    double conv_Delta_pdf_qg_order_1(void *vars);

    double DeltaPdfQuarkQuark(double scale,double x,int flavopr);

    double DeltaPdfQuarkGluon(double scale,double x,Parton flavor);

    double DeltaPdfCutoff(double scale,double x,Parton flavor);

    double PdfSchemeDepDirect(double scale, double x,Parton flavor, SchemeChoice scheme);

    double PdfSchemeDepGrid(double muQ0,double x,Parton flavor);//cutoff from MSbar, dropping p.s. terms, from precomputed lhapdf grids
    ///WARNING: there are not gluons here, so function returns some value for flavor=Parton::gluon but this is not usable
    double PdfSchemeDep(double muQ0,double x,Parton flavor, SchemeChoice scheme);

  }//collinear

}//hso

#endif//_PDF_H_
