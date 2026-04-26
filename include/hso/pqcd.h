#ifndef _PQCD_H_
#define _PQCD_H_

#include <map>
#include <utility>

namespace hso{
  ///math
  inline constexpr double Pi = 3.1415926535897932385E0;

  inline constexpr double gamma_euler = 0.5772156649015328606E0;
  //pdg Particle ids
  enum class Particle {
    Proton = 2212,          //hadrons
    Neutron = 2112,
    ProtonBar = -Proton,
    NeutronBar = -Neutron,
    Gluon = 21,             //gauge bosons
    Photon = 22,
    Z0 = 23,
    Down = 1,               //quarks
    Up = 2,
    Strange = 3,
    Charm = 4,
    Bottom = 5,
    Top = 6,
    DownBar = -Down,
    UpBar = -Up,
    StrangeBar = -Strange,
    CharmBar = -Charm,
    BottomBar = -Bottom,
    TopBar = -Top
  };

  enum class Parton {
    Down = static_cast<int>(Particle::Down),
    Up = static_cast<int>(Particle::Up),
    Strange = static_cast<int>(Particle::Strange),
    Charm = static_cast<int>(Particle::Charm),
    Bottom = static_cast<int>(Particle::Bottom),
    Top = static_cast<int>(Particle::Top),
    Gluon = static_cast<int>(Particle::Gluon),
    DownBar = static_cast<int>(Particle::DownBar),
    UpBar = static_cast<int>(Particle::UpBar),
    StrangeBar = static_cast<int>(Particle::StrangeBar),
    CharmBar = static_cast<int>(Particle::CharmBar),
    BottomBar = static_cast<int>(Particle::BottomBar),
    TopBar = static_cast<int>(Particle::TopBar),
  };

  inline std::map<Parton, double> Parton_charge {
    {Parton::Down, -1.0/3.0},
    {Parton::Up, 2.0/3.0},
    {Parton::Strange, -1.0/3.0},
    {Parton::Charm, 2.0/3.0},
    {Parton::Bottom, -1.0/3.0},
    {Parton::Top, 2.0/3.0},
    {Parton::Gluon, 0.0},
    {Parton::DownBar, 1.0/3.0},
    {Parton::UpBar, -2.0/3.0},
    {Parton::StrangeBar, 1.0/3.0},
    {Parton::CharmBar, -2.0/3.0},
    {Parton::BottomBar, 1.0/3.0},
    {Parton::TopBar, -2.0/3.0},
  };

  enum class target {//uses atomic number (number of protons) as value
    Beryllium9 = 4,
    Copper63 = 29,
    Platinum195 = 78
  };

  inline const std::map<target, int> nucleon_number {//number of nucleons of isotopes used in target experiments
    {target::Beryllium9, 9},
    {target::Copper63, 63},
    {target::Platinum195, 195}
  };

  enum class InState {
    Proton = static_cast<int>(Particle::Proton),              //hadrons
    Neutron = static_cast<int>(Particle::Neutron),
    ProtonBar = static_cast<int>(Particle::ProtonBar),
    NeutronBar = static_cast<int>(Particle::NeutronBar),
    Beryllium9 = static_cast<int>(target::Beryllium9),        //heavy targets use atomic number (charge)
    Copper63 = static_cast<int>(target::Copper63),
    Platinum195 = static_cast<int>(target::Platinum195)
  };

  enum class DrellYanChannel {
    Photon = static_cast<int>(Particle::Photon),
    Z0 = static_cast<int>(Particle::Z0),
    InterfPhotonZ0 = Photon * Z0,
    All = Photon + Z0 + InterfPhotonZ0
  };

  ///hadrons masses
  inline constexpr double Mp = 0.9383;
  inline constexpr double Mn = 0.9396;
  inline constexpr double Mpp = 0.1396;
  inline constexpr double Mpm = 0.1396;
  inline constexpr double Mp0 = 0.1350;
  inline constexpr double Mkp = 0.4937;
  inline constexpr double Mkm = 0.4937;
  inline constexpr double Mk0 = 0.4976;
  //masses according to lhapdf
  inline constexpr double gluon_mass = 0.0;
  inline constexpr double down_mass = 0.0;
  inline constexpr double up_mass = 0.0;
  inline constexpr double strange_mass = 0.5;
  inline constexpr double charm_mass = 1.3;
  inline constexpr double bottom_mass = 4.5;
  inline constexpr double top_mass = 174.0;
  ///qed
  inline constexpr double alpha_qed = (1.0/137);// at low energies
  //electro-weak
  inline constexpr double sin_theta_w_squared_MSbar = 0.23122; //at Z0 boson mass
  inline constexpr double cos_theta_w_squared_MSbar = 0.76878; //at Z0 boson mass
  inline constexpr double MZ = (91.1876);
  inline constexpr double Z0_boson_width = (2.4952);
  inline constexpr double zeta3 = 1.2020569031595942;
  inline constexpr double polygamma21= 2.4041138063191886;
  inline constexpr double CF = 1.3333333333333333;
  inline constexpr double TF = 0.5;
  inline constexpr double NC = 3;
  inline constexpr double nf = 3;//nf= reproduces AlphaStrong for jam20
  inline constexpr double CA = NC;
  inline constexpr double gammak1 = 8.*CF;
  inline constexpr double gammaF1 = 6*CF;
  inline constexpr double gammaF2 = CF*CF*(3 + 48*zeta3 - 4*Pi*Pi) + CF*nf*(-4.814814814814815 - (2*Pi*Pi)/3.) + CA*CF*(35.592592592592595 - 52*zeta3 + (11*Pi*Pi)/3.);
  inline constexpr double H1 = CF*(-16 + Pi*Pi*Pi/3.);
  inline constexpr double k1 = 0;
  inline constexpr double k02 = 8*CF*((14*nf)/27. + CA*(-3.740740740740741 + (7*zeta3)/2.));
  inline constexpr double beta0 = (11.0*CA)/3. - (2.0*nf)/3.;
  inline constexpr double beta1 = (34.0*CA*CA)/3. - 10./3.*CA*nf -2.0*CF*nf;
  inline constexpr double b0val = 1.122918967;
  inline constexpr double C1 = b0val;
  inline constexpr int MinFlav = -3;//WARNING: 3 ok. must test other values (transfrm. p->antip,p->n,p->target)
  inline constexpr int MaxFlav = 3;//WARNING:-3 ok. must test other values (transfrm. p->antip,p->n,p->target)


  double AlphaStrong(double scale);

  double as(double scale);

}//hso

#endif//_PQCD_H_
