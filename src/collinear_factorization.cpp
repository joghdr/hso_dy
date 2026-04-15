#include "collinear_factorization.h"
#include "behavior.h"
#include "pqcd.h"
#include "pdf_cutoff_from_msbar.h"
#include "gsl_settings.h"
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <cmath>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <vector> // IWYU pragma: keep
#include <LHAPDF/Factories.h>
#include <LHAPDF/PDF.h>



namespace hso{

  namespace collinear{

    GetCoefficients ABC_coeff_mode = GetCoefficients::FromGrids;

    LHAPDF::PDF *f_MSbar;

    LHAPDF::PDF *Apdf_lhapdf_pointer;

    LHAPDF::PDF *Apdf_g_lhapdf_pointer;

    LHAPDF::PDF *Bpdf_lhapdf_pointer;

    extern LHAPDF::PDF *fr1Oas1_lhapdf_pointer;

    void SetLhapdf(std::string lhapdf_set,int setid) {

      std::string AGRIDNAME ("ApdfOas1_"+lhapdf_set);

      std::string AGGRIDNAME ("Apdf_g_Oas1_"+lhapdf_set);

      std::string BGRIDNAME ("BpdfOas1_"+lhapdf_set);

      std::string FR1GRIDNAME("fr1Oas1_"+lhapdf_set);

      std::string FOPE1GRIDNAME("FopeOas1_"+lhapdf_set);

      f_MSbar = LHAPDF::mkPDF(lhapdf_set, setid);

      Apdf_lhapdf_pointer = LHAPDF::mkPDF(AGRIDNAME, setid);

      Apdf_g_lhapdf_pointer = LHAPDF::mkPDF(AGGRIDNAME, setid);

      Bpdf_lhapdf_pointer = LHAPDF::mkPDF(BGRIDNAME, setid);

      fr1Oas1_lhapdf_pointer = LHAPDF::mkPDF(FR1GRIDNAME, setid);

    }
    //MSbar collinear PDF from LHAPDF
    double Pdf(double scale, double x, Parton flavor,bool freeze){

      double output=0.0;

      double Q2MAXGRID=f_MSbar->q2Max();

      double Q2 = 0.0;

      if(freeze) Q2 = std::min( std::pow(scale, 2.0) , Q2MAXGRID );

      else Q2 = std::pow(scale, 2.0);

      double xf = f_MSbar->xfxQ2(static_cast<int>(flavor), x, Q2);

      output = xf/x;

      return output;

    }
    //NOTE: integrators for convolutions
    double Convolution2X(double (*integrand)(double , void*), void *vars){

      double output=0.0, error=0.0;

      double x = *static_cast<double**>(vars)[1];

      double pts[] = {x,1.0};

      int npts = 2;

      using namespace hso::gsl_default;

      gsl_integration_workspace* WX2 = gsl_integration_workspace_alloc(kGSLWIntervals);

      gsl_function FX2; //conv2

      FX2.function = integrand;

      FX2.params = vars;

      gsl_integration_qagp(&FX2, pts, npts, kGSLEpsAbs, kGSLEpsRel, kGSLLimit, WX2, &output, &error);

      gsl_integration_workspace_free(WX2);

      return output;

    }

    double Convolution3X(double (*integrand)(double , void*), void *vars){

      double output=0.0, error=0.0;

      double x = *static_cast<double**>(vars)[1];

      double pts[] = {x,1.0};

      int npts = 2;

      using namespace hso::gsl_default;

      gsl_integration_workspace* WX3 = gsl_integration_workspace_alloc(kGSLWIntervals);

      gsl_function FX3;

      FX3.function = integrand;

      FX3.params = vars;

      gsl_integration_qagp(&FX3, pts, npts, kGSLEpsAbs, kGSLEpsRel, kGSLLimit, WX3, &output, &error);

      gsl_integration_workspace_free(WX3);

      return output;

    }
    //NOTE collinear functions involved in pQCD tails of tmds
    double conv_A_tmdpdf_qq_order_1_integrand( double xi, void *vars){

      double output = 0.0;

      double scale = *static_cast<double**>(vars)[0];

      double x = *static_cast<double**>(vars)[1];

      Parton flavor = *static_cast<Parton**>(vars)[2];

      output = Pdf(scale,x*std::pow(xi,-1), flavor)*CF*(-1.0-xi)/xi;

      output += ( Pdf(scale,x*std::pow(xi,-1),flavor)/xi - Pdf(scale,x,flavor) )*CF* 2.0/(1.0-xi);

      return output;

    }

    double conv_A_tmdpdf_qq_order_1(void *vars){

      double output = 0.0;

      double scale = *static_cast<double**>(vars)[0];

      double x = *static_cast<double**>(vars)[1];

      Parton flavor = *static_cast<Parton**>(vars)[2];

      output = Convolution2X(conv_A_tmdpdf_qq_order_1_integrand,vars);

      output += 3.0/2.0*CF*Pdf(scale,x,flavor);//delta Piece

      output += 2.0*CF*std::log(1.0-x)*Pdf(scale,x,flavor);//plus distribution end point

      return output;

    }

    double conv_A_tmdpdf_qg_order_1_integrand( double xi, void *vars){

      double output = 0.0;

      double scale = *static_cast<double**>(vars)[0];

      double x = *static_cast<double**>(vars)[1];

      output = Pdf(scale,x*std::pow(xi,-1),Parton::Gluon)*TF*( xi*xi + std::pow(1.0-xi,2) );

      output /= xi;

      return output;

    }

    double conv_A_tmdpdf_qg_order_1(void *vars){

      double output = 0.0;

      output = Convolution2X(conv_A_tmdpdf_qg_order_1_integrand,vars);

      return output;

    }

    double ApdfQuarkQuarkDirect(double muQ0,double x, Parton flavor){//only for quark channel

      double output=0.0;

      void *vars[]={&muQ0, &x, &flavor};

      if(flavor != Parton::Gluon){

        output = conv_A_tmdpdf_qq_order_1(vars);

        output += -3.0/2.0 *CF * Pdf(muQ0,x,flavor);

        output *= AlphaStrong(muQ0)/Pi;

      }

      return output;

    }

    double ApdfQuarkGluonDirect(double muQ0,double x,Parton flavor){

      double output=0.0;

      void *vars[] = {&muQ0, &x, &flavor};

      if(flavor != Parton::Gluon){

        output = conv_A_tmdpdf_qg_order_1(vars);

        output *= AlphaStrong(muQ0)/Pi;

      }

      return output;

    }

    double ApdfQuarkQuarGrid(double muQ0,double x,Parton flavor){

      double output=0.0;

      double muQ02=pow(muQ0,2);

      output=Apdf_lhapdf_pointer->xfxQ2(static_cast<int>(flavor), x, muQ02);

      return output/x;

    }

    double ApdfQuarkGluonGrid(double muQ0, double x, Parton flavor){

      double output=0.0;

      double muQ02=pow(muQ0,2);

      output=Apdf_g_lhapdf_pointer->xfxQ2(static_cast<int>(flavor), x, muQ02);

      return output/x;

    }

    double ApdfQuarkQuark(double muQ0,double x,Parton flavor){//for function wrapper, now only for quark channel

      double output=0.0;

      switch(ABC_coeff_mode){

        case GetCoefficients::FromGrids:

          output=ApdfQuarkQuarGrid(muQ0, x, flavor);

          break;

        case GetCoefficients::ByDirectCalculation:

          output=ApdfQuarkQuarkDirect(muQ0, x, flavor);

          break;

        default:

          std::stringstream msg;

          msg<<"Error:\n\t"<<__FUNCTION__<<":\n\t\t"

          <<"ABC_coeff_mode != { GetCoefficients::ByDirectCalculation, GetCoefficients::FromGrids} allowed. Aborting\n\n";

          std::cout<<msg.str();

          std::exit(1);

      }

      return output;

    }

    double ApdfQuarkGluon(double muQ0,double x,Parton flavor){//for function wrapper, gluon channel

      double output=0.0;

      switch(ABC_coeff_mode){

        case GetCoefficients::FromGrids:

          output=ApdfQuarkGluonGrid(muQ0, x, flavor);

          break;

        case GetCoefficients::ByDirectCalculation:

          output=ApdfQuarkGluonDirect(muQ0, x, flavor);

          break;

        default:

          std::stringstream msg;

          msg<<"Error:\n\t"<<__FUNCTION__<<":\n\t\t"

          <<"ABC_coeff_mode != { GetCoefficients::ByDirectCalculation, GetCoefficients::FromGrids} allowed. Aborting\n\n";

          std::cout<<msg.str();

          std::exit(1);

      }

      return output;

    }

    double BpdfDirect(double muQ0,double x,Parton flavor){

      double output=0.0;

      if(flavor != Parton::Gluon){

        output = CF * Pdf(muQ0, x, flavor);

        output *= AlphaStrong(muQ0)/Pi;

      }

      return output;

    }

    double BpdfGrid(double muQ0,double x,Parton flavor){

      double output=0.0;

      double muQ02=pow(muQ0,2);

      output = Bpdf_lhapdf_pointer->xfxQ2(static_cast<int>(flavor), x, muQ02);

      return output/x;

    }

    double Bpdf(double muQ0,double x, Parton flavor){//for function wrapper

      double output=0.0;

      switch(ABC_coeff_mode){

        case GetCoefficients::FromGrids:

          output=BpdfGrid(muQ0,x,flavor);

          break;

        case GetCoefficients::ByDirectCalculation:

          output=BpdfDirect(muQ0,x,flavor);

          break;

        default:

          std::stringstream msg;

          msg<<"Error:\n\t"<<__FUNCTION__<<":\n\t\t"

          <<"ABC_coeff_mode != { GetCoefficients::ByDirectCalculation, GetCoefficients::FromGrids} allowed. Aborting\n\n";

          std::cout<<msg.str();

          std::exit(1);

      }

      return output;

    }

    double Cpdf(double muQ0,double Q0,double x,Parton flavor,double m){

      double output=0.0;

      SchemeChoice cutoff_scheme = SchemeChoice::CutoffFromMSbar;

      double fc_app = PdfSchemeDep(muQ0,x,flavor, cutoff_scheme);//always cutoff pdf scheme as it comes from the integral relation

      if(flavor != Parton::Gluon){

        output = fc_app;

        output -= ApdfQuarkQuark (muQ0,x,flavor)*std::log(muQ0/m);

        output -= ApdfQuarkGluon(muQ0,x,flavor)*std::log(muQ0/m);

        output -= Bpdf (muQ0,x,flavor)*std::log(muQ0/m)*std::log(Q0*Q0/muQ0/m);

      }

      return output;

    }

    double TmdPdfPert( double x, double muQ, double Q, double kT, Parton flavor ){

      double output=0.0;

      if(flavor == Parton::Gluon){std::cout<<"Error: collinear::TmdPdfPert: gluon f_pert not implemented."<<std::endl;std::exit(1);}

      double A = ApdfQuarkQuark(muQ,x,flavor);

      double A_g = ApdfQuarkGluon(muQ,x,flavor);

      double B = Bpdf(muQ,x,flavor);

      output = ((A + A_g + B*std::log(pow(kT,-2)*std::pow(Q,2)))*std::pow(kT,-2)*std::pow(Pi,-1))/2.;

      return output;

    }

  }//collinear

}//hso
