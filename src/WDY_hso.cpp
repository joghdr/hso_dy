#include "WDY_hso.h"
#include "pqcd.h"
#include "tmdPDF.h"
#include "evo.h"
#include "collinear_factorization.h"
#include "pdf_cutoff_from_msbar.h"
#include <vector>
#include <iostream>
#include <cmath>
//////////////////////////////////////////////////////////
////
//// NOTE: Only functions used for HSO DY pheno 2023
//// more functions can be found in 'WDY_all.h'
////
//////////////////////////////////////////////////////////
namespace hso{

  namespace drell_yan {

    double MaxLeptonRapidity(double s,double Q,double qT){

      double output=0.0;

      double rho=qT/Q;

      output = (1.0 + std::pow(Q,2)/s) * std::sqrt(s);

      output /= 2.*Q*std::sqrt(1. + std::pow(rho,2));

      output = std::acosh(output);

      return output;

    }

    double MinLeptonRapidity(double s,double Q,double qT){

      return -MaxLeptonRapidity(s,Q,qT);

    }

    std::vector<double> GetTmdHadron( std::vector<double> tmd_proton, InState hadron_id ){

      double tmd_proton_u =tmd_proton[0];

      double tmd_proton_ub=tmd_proton[1];

      double tmd_proton_d =tmd_proton[2];

      double tmd_proton_db=tmd_proton[3];

      double tmd_proton_s =tmd_proton[4];

      double tmd_proton_sb=tmd_proton[5];

      std::vector<double> tmd_hadron(tmd_proton.size());

      switch(hadron_id){

        case InState::Neutron ://neutron

          tmd_hadron[0]=tmd_proton_d ;// f_u /n

          tmd_hadron[1]=tmd_proton_db;// f_ub/n

          tmd_hadron[2]=tmd_proton_u ;// f_d /n

          tmd_hadron[3]=tmd_proton_ub;// f_db/n

          tmd_hadron[4]=tmd_proton_s ;// f_s /n

          tmd_hadron[5]=tmd_proton_sb;// f_sb/n

          break;

        case InState::ProtonBar ://antiproton

          tmd_hadron[0]=tmd_proton_ub;// f_u /n

          tmd_hadron[1]=tmd_proton_u ;// f_ub/n

          tmd_hadron[2]=tmd_proton_db;// f_d /n

          tmd_hadron[3]=tmd_proton_d ;// f_db/n

          tmd_hadron[4]=tmd_proton_sb;// f_s /n

          tmd_hadron[5]=tmd_proton_s ;// f_sb/n

          break;

        default: //proton

          tmd_hadron[0]=tmd_proton_u ;// f_u /p

          tmd_hadron[1]=tmd_proton_ub;// f_ub/p

          tmd_hadron[2]=tmd_proton_d ;// f_d /p

          tmd_hadron[3]=tmd_proton_db;// f_db/p

          tmd_hadron[4]=tmd_proton_s ;// f_s /p

          tmd_hadron[5]=tmd_proton_sb;// f_sb/p

      }

      return tmd_hadron;

    }

    std::vector<double> GetTmdTarget( std::vector<double> tmd_proton, InState target_in){

      std::vector<double> tmd_target( tmd_proton.size() );

      std::vector<double> tmd_neutron( GetTmdHadron(tmd_proton, InState::Neutron) );

      int tmd_target_size = static_cast<int>(tmd_target.size());

      int target_Z = static_cast<int>(target_in);

      int target_A = nucleon_number.at(static_cast<target>(target_in));

      for(int i = 0; i < tmd_target_size; i++){

        tmd_target[i] = ( target_Z * tmd_proton[i] + (target_A - target_Z )* tmd_neutron[i] ) / (target_A);

      }

      return tmd_target;

    }

    double HardFactorPhoton( double mu, double Q, Parton flavor){

      double output=0.0;

      double T=log(pow(Q/mu,2.0));

      double eq=Parton_charge.at(flavor);

      output = 1.0;

      output += as(mu)*CF*( -16.0 + 7.0/3.0*std::pow(Pi,2.0) + 6.0*T - 2.0*std::pow(T,2.0) );

      output *= std::pow(eq, 2);

      return output;

    }
    //does NOT contain the interference term
    double HardFactorZ0( double mu, double Q, Parton flavor){//WARNING: uses always Weinberg angle at M_Z0

      double output=0.0;

      double T=log(pow(Q/mu,2.0));

      double sw2=sin_theta_w_squared_MSbar;//for weak boson

      double cw2=cos_theta_w_squared_MSbar;//for weak boson

      double eq=Parton_charge.at(flavor);

      output = 1.0;

      output += as(mu)*CF*( -16.0 + 7.0/3.0*std::pow(Pi,2.0) + 6.0*T - 2.0*std::pow(T,2.0) );

      output *= std::pow(Q, 4);

      output /= std::pow( std::pow(MZ,2) - std::pow(Q,2) , 2 ) + std::pow(MZ,2) * std::pow(Z0_boson_width,2);

      output *= (1. + std::pow(1. - 4.*sw2,2))/(16.*cw2*sw2);

      output *= (1. + std::pow(1. - 4.*sw2*std::abs(eq),2))/(16.*cw2*sw2);

      return output;

    }

    double HardFactorInterfPhotonZ0( double mu, double Q, Parton flavor){//uses always Weinberg angle at M_Z0

      double output=0.0;

      double T=log(pow(Q/mu,2.0));

      double sw2=sin_theta_w_squared_MSbar;

      double cw2=cos_theta_w_squared_MSbar;

      double eq=Parton_charge.at(flavor);

      output = 1.0;

      output += as(mu)*CF*( -16.0 + 7.0/3.0*std::pow(Pi,2.0) + 6.0*T - 2.0*std::pow(T,2.0) );

      output *= std::pow(Q, 2);

      output *= (std::pow(Q, 2) - std::pow(MZ, 2));

      output /= (std::pow( std::pow(MZ,2) - std::pow(Q,2) , 2 ) + std::pow(MZ,2) * std::pow(Z0_boson_width,2) );

      output *= std::abs(eq);

      output *= (1. - 4.*sw2);

      output *= (1. - 4.*sw2*std::abs(eq))/(8.*cw2*sw2);

      return output;

    }
    //hard factor wrapper function to choose channel
    //for observables: Photon, All
    //for tests: Z0 (no interference term), InterfPhotonZ0 (only interference term)
    double HardFactor( double mu, double Q, Parton flavor , DrellYanChannel channel){

      double output=0.0;

      switch(channel){

        case DrellYanChannel::Photon:

          output = HardFactorPhoton( mu, Q, flavor);

          break;

        case DrellYanChannel::Z0:

          output = HardFactorZ0( mu, Q, flavor);

          break;

        case DrellYanChannel::InterfPhotonZ0:

          output = HardFactorInterfPhotonZ0(mu, Q, flavor);

          break;

        case DrellYanChannel::All: //photon + Z0 + interf photon-Z0

          output = HardFactorPhoton(mu, Q, flavor) +

                   HardFactorZ0( mu, Q, flavor) +

                   HardFactorInterfPhotonZ0( mu, Q, flavor);

          break;

        default://photon

          output = HardFactorPhoton( mu, Q, flavor);

      }

      return output;

    }
    //W term with input functions (no RG improvements)
    double WbtInput(double bT, double xa, double xb,double muQ0,double Q0,double muQ,double Q,

                     void *Ftilde_a_para, void *Ftilde_b_para , void *Ktildepara,

                     InState hadron_a_id,InState hadron_b_id, DrellYanChannel channel){

      double output=0.0;

      std::vector<double> tmd_proton_a(0);

      std::vector<double> tmd_proton_b(0);

      std::vector<double> HardFactorval(0);

      std::vector<double> tmd_a(0);

      std::vector<double> tmd_b(0);

      std::vector<Parton> Flavors {//TODO: generalize (should also modify 'GetTmdHadron' and 'GetTmdTarget')
        Parton::Up,
        Parton::UpBar,
        Parton::Down,
        Parton::DownBar,
        Parton::Strange,
        Parton::StrangeBar
      };

      for(Parton flavor: Flavors){

        tmd_proton_a.push_back(FtildeInputOrder_1_msbar(xa, bT, muQ0, Q0, flavor, Ftilde_a_para));

        tmd_proton_b.push_back(FtildeInputOrder_1_msbar(xb, bT, muQ0, Q0, flavor, Ftilde_b_para));

        HardFactorval.push_back(HardFactor(muQ, Q, flavor, channel));

      }

      switch(hadron_a_id){

        case InState::Proton:

          tmd_a = tmd_proton_a;

          break;

        case InState::ProtonBar:

          tmd_a = GetTmdHadron(tmd_proton_a, hadron_a_id);

          break;

        case InState::Copper63:

        case InState::Platinum195:

        case InState::Beryllium9:

          tmd_a = GetTmdTarget(tmd_proton_a, hadron_a_id);

          break;

        default:

          tmd_a = tmd_proton_a;

      }

      switch(hadron_b_id){

        case InState::Proton:

          tmd_b = tmd_proton_b;

          break;

        case InState::ProtonBar:

          tmd_b = GetTmdHadron(tmd_proton_b, hadron_b_id);

          break;

        case InState::Copper63:

        case InState::Platinum195:

        case InState::Beryllium9:

          tmd_b = GetTmdTarget(tmd_proton_b, hadron_b_id);

          break;

        default:

          tmd_b = tmd_proton_b;

      }

      int flavors_size = static_cast<int>(Flavors.size());

      for (int i = 0; i < flavors_size / 2; i++) {

        output += HardFactorval[2*i ] * tmd_a[2*i ] * tmd_b[2*i+1];

        output += HardFactorval[2*i+1] * tmd_a[2*i+1] * tmd_b[2*i ];

      }

      double evolution = WTermEvolutionNoRG(bT,muQ0, Q0, muQ, Q, Ktildepara);

      output *= evolution;

      return output;

    }
    //W term with underline functions (RG improvements)
    double WbtUnderline(double bT, double xa, double xb,double muQ0,double Q0,double muQ,double Q,

                        void *Ftilde_a_para, void *Ftilde_b_para ,void *Ktildepara, double rg_transf_Qmax,

                        InState hadron_a_id, InState hadron_b_id, DrellYanChannel channel){//proton-proton default. Hadron id for hadron or target

      double output=0.0;

      std::vector<double> tmd_proton_a(0);

      std::vector<double> tmd_proton_b(0);

      std::vector<double> HardFactorval(0);

      std::vector<double> tmd_a(0);

      std::vector<double> tmd_b(0);//hadron or target

      std::vector<Parton> Flavors {//TODO: generalize (should also modify 'GetTmdHadron' and 'GetTmdTarget')
        Parton::Up,
        Parton::UpBar,
        Parton::Down,
        Parton::DownBar,
        Parton::Strange,
        Parton::StrangeBar
      };

      for(Parton flavor: Flavors){

        tmd_proton_a.push_back(FtildeUnderlineOrder_1_msbar(xa,bT,muQ0,Q0,flavor,

                                                            Ftilde_a_para,Ktildepara,rg_transf_Qmax));

        tmd_proton_b.push_back(FtildeUnderlineOrder_1_msbar(xb,bT,muQ0,Q0,flavor,

                                                            Ftilde_b_para,Ktildepara,rg_transf_Qmax));

        HardFactorval.push_back(HardFactor(muQ, Q, flavor , channel));

      }

      switch(hadron_a_id){

        case InState::Proton:

          tmd_a = tmd_proton_a;

          break;

        case InState::ProtonBar:

          tmd_a = GetTmdHadron(tmd_proton_a, hadron_a_id);

          break;

        case InState::Copper63:

        case InState::Platinum195:

        case InState::Beryllium9:

          tmd_a = GetTmdTarget(tmd_proton_a, hadron_a_id);

          break;

        default:

          tmd_a = tmd_proton_a;

      }

      switch(hadron_b_id){

        case InState::Proton:

          tmd_b = tmd_proton_b;

          break;

        case InState::ProtonBar:

          tmd_b = GetTmdHadron(tmd_proton_b, hadron_b_id);

          break;

        case InState::Copper63:

        case InState::Platinum195:

        case InState::Beryllium9:

          tmd_b = GetTmdTarget(tmd_proton_b, hadron_b_id);

          break;

        default:

          tmd_b = tmd_proton_b;

      }

      int flavors_size = static_cast<int>(Flavors.size());

      for(int i = 0; i < flavors_size / 2; i++) {

        output += HardFactorval[2*i ] * tmd_a[2*i ] * tmd_b[2*i+1];

        output += HardFactorval[2*i+1] * tmd_a[2*i+1] * tmd_b[2*i ];

      }

      double evolution = WTermEvolution(bT,muQ0, Q0, muQ, Q,Ktildepara, rg_transf_Qmax);

      output *= evolution;

      return output;

    }

  }//drell_yan

}//hso
