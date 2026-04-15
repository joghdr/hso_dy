#include "csec_DY_css_hso_integrands_cuba.h"
#include "pqcd.h"
#include "WDY_hso.h"
#include <gsl/gsl_sf_bessel.h>
#include <vector>
#include <cmath>

namespace hso{

  namespace csec_no_rg_improved {
    ///@prototype: fixed target E288 experiment; units -> fb * GeV^-2
    int integrand_drell_yan_1(

      const int *, const double xx[],

      const int *, double ff[], void *P

    ) {

      std::vector< int> behavior = *static_cast<std::vector< int>**>(P)[0];

      std::vector<double> csecvars = *static_cast<std::vector<double>**>(P)[1];

      std::vector<double> bins_min = *static_cast<std::vector<double>**>(P)[2];

      std::vector<double> bins_max = *static_cast<std::vector<double>**>(P)[3];

      void *scales =static_cast<void**>(P)[5];

      void *Ftilde_a_para=static_cast<void**>(P)[6];

      void *Ftilde_b_para=static_cast<void**>(P)[7];

      void *Ktildepara =static_cast<void**>(P)[8];

      double T_Q =xx[1];

      double T_bT=xx[0];

      double Qmin = bins_min[0], Qmax = bins_max[0];

      double Q = T_Q *( Qmax- Qmin)+ Qmin;

      double bT = (1.-T_bT)/T_bT;

      double yh =csecvars[0];

      double sqrt_s =csecvars[1];

      double qT =csecvars[2];

      double s =sqrt_s*sqrt_s;

      double Q0 =*static_cast<double**>(scales)[0];

      double mu_over_Q0 =*static_cast<double**>(scales)[2];

      double sqrt_zeta_over_Q0 =*static_cast<double**>(scales)[3];

      double mu_over_Q =*static_cast<double**>(scales)[4];

      double sqrt_zeta_over_Q =*static_cast<double**>(scales)[5];

      double cuba_jacobian=1.0;

      cuba_jacobian *= ( Qmax- Qmin );

      cuba_jacobian *= std::pow(1. + bT,2.);

      double jacobian=1.0 ;

      jacobian *= 1.0/Pi;

      jacobian *= 2.0*Q;

      double units=389000000000.;//GeV^-4 to fb * GeV^-2

      double bin_volume=1.0;

      double factor=cuba_jacobian*jacobian*units/bin_volume;

      double output;

      //scales passed to W term
      double muQ0 =Q0* mu_over_Q0;

      double sqrt_zetaQ0 =Q0* sqrt_zeta_over_Q0;

      double muQ =Q * mu_over_Q;

      double sqrt_zetaQ =Q * sqrt_zeta_over_Q;

      double xa=Q/sqrt(s)*std::exp( yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double xb=Q/sqrt(s)*std::exp(-yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double kin=1.0;

      kin = 4.0*std::pow(Pi,2.0)*std::pow(alpha_qed,2.)/9.0/pow(Q,2.0)/s;//W-term to dsigma_dy_dQ2_dqT2 (after ft)

      output =bT * gsl_sf_bessel_J0(bT*qT)/2./Pi;

      output *=drell_yan::WbtInput(bT, xa, xb,muQ0,sqrt_zetaQ0,muQ,sqrt_zetaQ,

                                    Ftilde_a_para,Ftilde_b_para , Ktildepara,

                                    InState::Proton,InState::Copper63);

      output *=kin;

      ff[0]=output*factor;

      return 0;

    }
    ///@prototype: fixed target E605 experiment; units -> pb * GeV^-2
    int integrand_drell_yan_4_xFavg_qTavg (

      const int *, const double xx[],

      const int *, double ff[], void *P

    ) {

      std::vector< int> behavior = *static_cast<std::vector< int>**>(P)[0];

      std::vector<double> csecvars = *static_cast<std::vector<double>**>(P)[1];

      std::vector<double> bins_min = *static_cast<std::vector<double>**>(P)[2];

      std::vector<double> bins_max = *static_cast<std::vector<double>**>(P)[3];

      void *scales =static_cast<void**>(P)[5];

      void *Ftilde_a_para=static_cast<void**>(P)[6];

      void *Ftilde_b_para=static_cast<void**>(P)[7];

      void *Ktildepara =static_cast<void**>(P)[8];

      double T_Q = xx[1];

      double T_bT = xx[0];

      double Qmin = bins_min[0];

      double Qmax = bins_max[0];

      double Q = T_Q *( Qmax- Qmin)+ Qmin;

      double bT = (1.-T_bT)/T_bT;

      double xF =csecvars[0];

      double sqrt_s =csecvars[1];

      double qT =csecvars[2];

      double Q2avg=pow( (Qmin+Qmax)/2.0 , 2 );

      double yh = std::asinh(sqrt_s/2.0/Q*xF );//qT=0

      double s =sqrt_s*sqrt_s;

      double Q0 =*static_cast<double**>(scales)[0];
      //TODO: implement general scales as in csec_no_rg_improved:: integrand_drell_yan_1
      double cuba_jacobian=1.0;

      cuba_jacobian *= ( Qmax- Qmin );

      cuba_jacobian *= std::pow(1. + bT,2.);

      double jacobian=1.0;

      jacobian *= 1.0/Pi;

      jacobian *= 2.0*Q;

      jacobian *= 1.0 / std::sqrt(xF*xF+4.0*Q*Q/s);//from dyh to dxF with qT=0
      //extra factor outside integral in O4
      double avg_jacobian = 2.0*std::sqrt( Q2avg + xF*xF * s/4.0 ) / sqrt_s;

      double units=389000000.;//GeV^-4 to pb * GeV^-2

      double bin_volume=1.0;

      double factor=cuba_jacobian*jacobian*units/bin_volume;

      double output;

      double muQ0=Q0,muQ=Q;

      double xa=Q/sqrt(s)*std::exp( yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double xb=Q/sqrt(s)*std::exp(-yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double kin=1.0;

      kin = 4.0*std::pow(Pi,2.0)*std::pow(alpha_qed,2.)/9.0/pow(Q,2.0)/s;//W-term to dsigma_dy_dQ2_dqT2 (after ft)

      output =bT * gsl_sf_bessel_J0(bT*qT)/2./Pi;

      output *=drell_yan::WbtInput(bT, xa, xb,muQ0,Q0,muQ,Q,

                                    Ftilde_a_para,Ftilde_b_para , Ktildepara,

                                    InState::Proton,InState::Copper63);

      output *=kin;

      ff[0]=output*factor*avg_jacobian;

      return 0;

    }

  }//csec_no_rg_improved

  namespace csec_rg_improved {

    int integrand_drell_yan_1(

      const int *, const double xx[],

      const int *, double ff[], void *P

    ) {

      std::vector< int> behavior = *static_cast<std::vector< int>**>(P)[0];

      std::vector<double> csecvars = *static_cast<std::vector<double>**>(P)[1];

      std::vector<double> bins_min = *static_cast<std::vector<double>**>(P)[2];

      std::vector<double> bins_max = *static_cast<std::vector<double>**>(P)[3];

      void *scales =static_cast<void**>(P)[5];

      void *Ftilde_a_para=static_cast<void**>(P)[6];

      void *Ftilde_b_para=static_cast<void**>(P)[7];

      void *Ktildepara =static_cast<void**>(P)[8];

      double T_Q =xx[1];

      double T_bT=xx[0];

      double Qmin = bins_min[0], Qmax = bins_max[0];

      double Q = T_Q *( Qmax- Qmin)+ Qmin;

      double bT = (1.-T_bT)/T_bT;

      double yh =csecvars[0];

      double sqrt_s =csecvars[1];

      double qT =csecvars[2];

      double s =sqrt_s*sqrt_s;

      double Q0 =*static_cast<double**>(scales)[0];

      double Q0_a=*static_cast<double**>(scales)[1];
      //TODO: implement general scales as in csec_no_rg_improved:: integrand_drell_yan_1
      double cuba_jacobian=1.0;

      cuba_jacobian *= ( Qmax- Qmin );

      cuba_jacobian *= std::pow(1. + bT,2.);

      double jacobian=1.0 ;

      jacobian *= 1.0/Pi;

      jacobian *= 2.0*Q;

      double units=389000000000.;//GeV^-4 to fb * GeV^-2

      double bin_volume=1.0;

      double factor=cuba_jacobian*jacobian*units/bin_volume;

      double output;

      double muQ0=Q0,muQ=Q;

      double xa=Q/sqrt(s)*std::exp( yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double xb=Q/sqrt(s)*std::exp(-yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double kin=1.0;

      kin = 4.0*std::pow(Pi,2.0)*std::pow(alpha_qed,2.)/9.0/pow(Q,2.0)/s;//W-term to dsigma_dy_dQ2_dqT2 (after ft)

      output =bT * gsl_sf_bessel_J0(bT*qT)/2./Pi;

      output *=drell_yan::WbtUnderline(bT, xa, xb,muQ0,Q0,muQ,Q,

                                        Ftilde_a_para, Ftilde_b_para ,Ktildepara, Q0_a,

                                        InState::Proton, InState::Copper63,DrellYanChannel::Photon);

      output *=kin;

      ff[0]=output*factor;

      return 0;

    }

    int integrand_drell_yan_4_xFavg_qTavg(

      const int *, const double xx[],

      const int *, double ff[], void *P

    ) {

      std::vector< int> behavior = *static_cast<std::vector< int>**>(P)[0];

      std::vector<double> csecvars = *static_cast<std::vector<double>**>(P)[1];

      std::vector<double> bins_min = *static_cast<std::vector<double>**>(P)[2];

      std::vector<double> bins_max = *static_cast<std::vector<double>**>(P)[3];

      void *scales =static_cast<void**>(P)[5];

      void *Ftilde_a_para=static_cast<void**>(P)[6];

      void *Ftilde_b_para=static_cast<void**>(P)[7];

      void *Ktildepara =static_cast<void**>(P)[8];

      double T_Q =xx[1];

      double T_bT=xx[0];

      double Qmin = bins_min[0], Qmax = bins_max[0];

      double Q = T_Q *( Qmax- Qmin)+ Qmin;

      double bT = (1.-T_bT)/T_bT;

      double xF =csecvars[0];

      double sqrt_s =csecvars[1];

      double qT =csecvars[2];

      double Q2avg=pow( (Qmin+Qmax)/2.0 , 2 );

      double yh=asinh(sqrt_s/2.0/Q*xF );//qT=0

      double s=sqrt_s*sqrt_s;

      double Q0=*static_cast<double**>(scales)[0];

      double Q0_a=*static_cast<double**>(scales)[1];
      //TODO: implement general scales as in csec_no_rg_improved:: integrand_drell_yan_1
      double cuba_jacobian=1.0;

      cuba_jacobian *= ( Qmax- Qmin );

      cuba_jacobian *= std::pow(1. + bT,2.);

      double jacobian=1.0;

      jacobian *= 1.0/Pi;

      jacobian *= 2.0*Q;

      jacobian *= 1.0 / std::sqrt( xF*xF + 4.0*Q*Q/s );//from dyh to dxF with qT=0
      //extra factor outside integral in O4
      double avg_jacobian = 2.0*std::sqrt( Q2avg + xF*xF * s/4.0 ) / sqrt_s;

      double units=389000000.;//GeV^-4 to pb * GeV^-2

      double bin_volume=1.0;

      double factor=cuba_jacobian*jacobian*units/bin_volume;

      double output;

      double muQ0=Q0,muQ=Q;

      double xa=Q/sqrt(s)*std::exp( yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double xb=Q/sqrt(s)*std::exp(-yh)/sqrt(1.0+std::pow(qT/Q,2.0));

      double kin=1.0;

      kin = 4.0*std::pow(Pi,2.0)*std::pow(alpha_qed,2.)/9.0/pow(Q,2.0)/s;//W-term to dsigma_dy_dQ2_dqT2 (after ft)

      output =bT * gsl_sf_bessel_J0(bT*qT)/2./Pi;

      output *=drell_yan::WbtUnderline(bT, xa, xb,muQ0,Q0,muQ,Q,

                                        Ftilde_a_para, Ftilde_b_para ,Ktildepara, Q0_a,

                                        InState::Proton , InState::Copper63 ,DrellYanChannel::Photon);

      output *=kin;

      ff[0]=output*factor*avg_jacobian;

      return 0;

    }
    ///@prototype: CDFI ZPT 1991 measurement
    //use with full Z0 propagator: must integrate over Q range
    int integrand_drell_yan_Z0_1(

      const int *, const double xx[],

      const int *, double ff[], void *P

    ) {

      std::vector< int> behavior = *static_cast<std::vector< int>**>(P)[0];

      std::vector<double> csecvars = *static_cast<std::vector<double>**>(P)[1];

      std::vector<double> bins_min = *static_cast<std::vector<double>**>(P)[2];

      std::vector<double> bins_max = *static_cast<std::vector<double>**>(P)[3];

      void *scales =static_cast<void**>(P)[5];

      void *Ftilde_a_para=static_cast<void**>(P)[6];

      void *Ftilde_b_para=static_cast<void**>(P)[7];

      void *Ktildepara =static_cast<void**>(P)[8];

      double T_bT=xx[0];

      double T_yh=xx[1];

      double T_Q=xx[2];

      double sqrt_s =csecvars[0];

      double s =sqrt_s*sqrt_s;

      double qT =csecvars[1];
      // rapidity yh limits are not passed, done by setting behaviour to VariabeType::Undetermined
      double Qmin = bins_min[0], Qmax = bins_max[0];

      double bT = (1.-T_bT)/T_bT;

      double Q = T_Q *( Qmax- Qmin)+ Qmin;

      double yhmin = drell_yan::MinLeptonRapidity(s,Q,qT), yhmax = drell_yan::MaxLeptonRapidity(s,Q,qT);

      double yh = T_yh*(yhmax-yhmin)+yhmin;

      double Q0 =*static_cast<double**>(scales)[0];

      double Q0_a=*static_cast<double**>(scales)[1];

      //TODO: implement general scales as in csec_no_rg_improved:: integrand_drell_yan_1
      // int channels=All;//tevatron
      double cuba_jacobian=1.0;

      cuba_jacobian *= (Qmax-Qmin);

      cuba_jacobian *= (yhmax-yhmin);

      cuba_jacobian *= std::pow(1. + bT,2.);

      double jacobian=1.0;

      jacobian *= 2.0*Q ;

      jacobian *= 2.0*qT;

      double units=389000000.;//GeV^-3 to pb * GeV

      double bin_volume = 1.0;

      double factor=cuba_jacobian*jacobian*units/bin_volume;

      double output;

      double muQ0=Q0,muQ=Q;

      double xa=Q/sqrt(s)*std::exp( yh)/sqrt(1.0+std::pow(qT/Q,2.0));//NOTE: need exact expression for exact ymin_min ymin_max

      double xb=Q/sqrt(s)*std::exp(-yh)/sqrt(1.0+std::pow(qT/Q,2.0));//NOTE: need exact expression for exact ymin_min ymin_max

      double kin=1.0;

      kin = 4.0*std::pow(Pi,2.0)*std::pow(alpha_qed,2.)/9.0/pow(Q,2.0)/s;//W-term to dsigma_dy_dQ2_dqT2 (after ft)

      output =bT * gsl_sf_bessel_J0(bT*qT)/2./Pi;

      output *= drell_yan::WbtUnderline(bT, xa, xb,muQ0,Q0,muQ,Q,

                                         Ftilde_a_para, Ftilde_b_para ,Ktildepara, Q0_a,

                                         InState::ProtonBar , InState::Proton ,DrellYanChannel::All);//NOTE:use RG impv for p pbar -> Z0 -> e+e-

      output *=kin;

      ff[0]=output*factor;

      return 0;

    }

  }//csec_rg_improved

}//hso
