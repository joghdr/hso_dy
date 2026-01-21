#include "pdf_cutoff_from_msbar.h"
#include "behavior.h"
#include "pqcd.h"
#include "gsl_settings.h"
#include "LHAPDF/LHAPDF.h"
#include <gsl/gsl_integration.h>
#include <iostream>
#include <cstdlib>
#include <cmath>

namespace hso{

  namespace collinear{

    LHAPDF::PDF* fr1Oas1_lhapdf_pointer;

    double Pdf(double scale, double x, Parton flavor,bool freeze=false);//defined in collinear_factorization.h

    double ConvolutionX (double (*integrand)( double , void *), void *vars) {

      double output=0.0;

      double error=0.0;

      double x = *static_cast<double**>(vars)[1];

      double pts[] = {x, 1.0};

      int npts = 2;

      using namespace hso::gsl_default;

      gsl_integration_workspace *WX = gsl_integration_workspace_alloc(kGSLWIntervals);

      gsl_function FX; //conv1

      FX.function = integrand;

      FX.params = vars;

      gsl_integration_qagp(&FX, pts, npts, kGSLEpsAbs, kGSLEpsRel, kGSLLimit, WX, &output, &error);

      gsl_integration_workspace_free(WX);

      return output;

    }

    double conv_Delta_pdf_qq_order_1_integrand( double xi, void *vars){//for approx version of cutoff-pdf using grids (MSbar).

      double output = 0.0;

      double scale = *static_cast<double**>(vars)[0];

      double x = *static_cast<double**>(vars)[1];

      Parton flavor = *static_cast<Parton**>(vars)[2];

      output = Pdf(scale,x*std::pow(xi,-1),flavor);

      output *= CF*(1.0-xi);

      output /= xi;

      return output;

    }//Conv_qq_pdf_Delta_MSbar_to_c_1_integrand

    double conv_Delta_pdf_qq_order_1(void *vars){

      double output = 0.0;

      double scale = *static_cast<double**>(vars)[0];

      double x = *static_cast<double**>(vars)[1];

      Parton flavor = *static_cast<Parton**>(vars)[2];

      output = ConvolutionX(conv_Delta_pdf_qq_order_1_integrand,vars);

      output += -Pi*Pi*CF/12.0 * Pdf(scale,x,flavor);//for mainstream MSbar

      return output;

    }//Conv_qq_pdf_Delta_MSbar_to_c_1

    double conv_Delta_pdf_qg_order_1_integrand( double xi, void *vars){//for approx version of dc using grids (MSbar), uses pdf then.

      double output = 0.0;

      double scale = *static_cast<double**>(vars)[0];

      double x = *static_cast<double**>(vars)[1];

      output = Pdf(scale,x*std::pow(xi,-1), Parton::Gluon);

      output *= 2.0*TF*xi*(1.0-xi);

      output /= xi;

      return output;

    }//Conv_qg_pdf_Delta_MSbar_to_c_1_integrand

    double conv_Delta_pdf_qg_order_1(void *vars){

      double output = 0.0;

      output = ConvolutionX(conv_Delta_pdf_qg_order_1_integrand,vars);

      return output;

    }//Conv_qg_pdf_Delta_MSbar_to_c_1

    double DeltaPdfQuarkQuark(double scale,double x,Parton flavor){//new: separeted q and g channels

      double output=0.0;

      void *vars[] = {
        &scale,
        &x,
        &flavor
      };

      output = conv_Delta_pdf_qq_order_1(vars);

      output *= AlphaStrong(scale)/2.0/Pi;

      return output;

    }

    double DeltaPdfQuarkGluon(double scale,double x,Parton flavor) {//new: separeted q and g channels

      double output=0.0;

      void *vars[] = {
        &scale,
        &x,
        &flavor
      };

      output = conv_Delta_pdf_qg_order_1(vars);

      output *= AlphaStrong(scale)/2.0/Pi;

      return output;

    }

    double DeltaPdfCutoff(double scale,double x,Parton flavor){//original: both q and g channels combined

      double output=0.0;

      void *vars[] = {
        &scale,
        &x,
        &flavor
      };

      output = conv_Delta_pdf_qq_order_1(vars);

      output += conv_Delta_pdf_qg_order_1(vars);

      output *= AlphaStrong(scale)/2.0/Pi;

      return output;

    }

    double PdfSchemeDepDirect(double scale, double x,Parton flavor, SchemeChoice scheme){

      double output=0.0;

      switch(scheme){

        case SchemeChoice::MSBar:

          output = Pdf(scale, x, flavor);//from lhapdf

          break;

        case SchemeChoice::CutoffFromMSbar:

          output = Pdf(scale, x, flavor);

          output += DeltaPdfQuarkQuark(scale, x, flavor);

          output += DeltaPdfQuarkGluon(scale, x, flavor);

          break;

        default:

          std::cout<<"error: scheme!={SchemeChoice::MSBar, SchemeChoice::CutoffFromMSbar} "

                   <<"not implemented."<<std::endl;

          std::exit(1);

      }

      return output;

    }

    double PdfSchemeDepGrid(double muQ0,double x,Parton flavor){

      double output=0.0;

      double muQ02=pow(muQ0,2);

      output=fr1Oas1_lhapdf_pointer->xfxQ2(static_cast<int>(flavor), x, muQ02);

      return output/x;

    }//cutoff from msbar O(alpha_s), dropping p.s. terms
    ///WARNING: there are not gluons here, so function returns some value for flavor = Parton::Gluon but this is not usable
    double PdfSchemeDep(double muQ0,double x,Parton flavor, SchemeChoice scheme){//for function wrapper

      double output=0.0;

      switch(scheme){

        case SchemeChoice::MSBar:

          output=Pdf(muQ0, x, flavor);

          break;

        case SchemeChoice::CutoffFromMSbar:

          switch(ABC_coeff_mode){

            case GetCoefficients::FromGrids:

              output=PdfSchemeDepGrid(muQ0,x,flavor);

              break;

            case GetCoefficients::ByDirectCalculation:

              output=PdfSchemeDepDirect(muQ0,x,flavor, SchemeChoice::CutoffFromMSbar);

              break;

            default:

              std::stringstream msg;

              msg<<"Error:\n\t"

              <<__FUNCTION__<<":\n\t\t"

              <<"ABC_coeff_mode != { GetCoefficients::ByDirectCalculation,"

              <<" GetCoefficients::FromGrids} allowed. Aborting\n\n";

              std::cout<<msg.str();

              std::exit(1);

          };//switch(ABC_coeff_mode)

          break;

            default:

              std::cout<<"error: scheme!={SchemeChoice::MSBar,"

              <<" SchemeChoice::CutoffFromMSbar} not implemented."<<std::endl;

              std::exit(1);

      };//switch(scheme)

      return output;

    }

  }//collinear

}//hso
