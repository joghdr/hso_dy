#ifndef _EVO_H_
#define _EVO_H_

namespace hso{

  double TMDEvolutionNoRG(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara);

  double WTermEvolutionNoRG(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara);

  double TMDEvolution(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara, double rg_transf_Qmax=-1.0);

  double WTermEvolution(double bT,double muQ0, double Q0, double muQ, double Q, void *Ktildepara, double rg_transf_Qmax=-1.0);

}//hso

#endif//_EVO_H_
