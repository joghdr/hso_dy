#ifndef _WDY_HSO_H_
#define _WDY_HSO_H_

#include "pqcd.h"
#include <vector>
////////////////////////////////////////////////////////
//// 
//// NOTE: Only functions used for HSO Drell-Yan pheno 2023.
//// more functions can be found in 'WDY_all.h',
//// which is set to do p + p -> gamma -> l+ + l- + X only
//// i.e. no heavy target or p + pbar DY.
//// 
/////////////////////////////////////////////////////////

namespace hso{

  namespace drell_yan {

    double MaxLeptonRapidity(double s,double Q,double qT);

    double MinLeptonRapidity(double s,double Q,double qT);

    std::vector<double> GetTmdHadron( std::vector<double> tmd_proton, InState hadron_id );

    std::vector<double> GetTmdTarget( std::vector<double> tmd_proton, InState target_in);

    double HardFactorPhoton( double mu, double Q, Parton flavor);

    double HardFactorZ0( double mu, double Q, Parton flavor);//does NOT contain the interference term

    double HardFactorInterfPhotonZ0( double mu, double Q, Parton flavor);
    //hard factor wrapper function to choose channel
    double HardFactor(double mu, double Q, Parton flavor,
                       DrellYanChannel channel=DrellYanChannel::Photon);

    double WbtInput(double bT, double xa, double xb,double muQ0,double Q0,double muQ,double Q,
                     void *Ftilde_a_para, void *Ftilde_b_para , void *Ktildepara,
                     InState hadron_a_id=InState::Proton,
                     InState hadron_b_id=InState::Proton,
                     DrellYanChannel channel=DrellYanChannel::Photon);//W term with input functions (no RG improvements)

    double WbtUnderline(double bT, double xa, double xb,double muQ0,double Q0,double muQ,double Q,
                          void *Ftilde_a_para, void *Ftilde_b_para ,void *Ktildepara, double rg_transf_Qmax,
                          InState hadron_a_id=InState::Proton,
                          InState hadron_b_id=InState::Proton,
                          DrellYanChannel channel=DrellYanChannel::Photon);//W term with underline functions (RG improvements)

  }//drell_yan

}//hso

#endif//_WDY_HSO_H_
