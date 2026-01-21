#ifndef _TMDPDF_H_
#define _TMDPDF_H_

#include "pqcd.h"
#include "LHAPDF/LHAPDF.h"

namespace hso{
  ///@global
  extern LHAPDF::PDF* Apdf_lhapdf_pointer;

  extern LHAPDF::PDF* Apdf_g_lhapdf_pointer;

  extern LHAPDF::PDF* Bpdf_lhapdf_pointer;

  extern double FtildeCore(double x, double bT, void *Ftildepara);
  ///@tmd_input_bT
  double FtildeInputOrder_1_msbar(double x, double bT,double muQ0,double Q0,
                                  Parton flavor,void *Ftildepara);
  //uses same ratio muQ0/Q0 for muQ0b/Q0b
  double FtildeUnderlineOrder_1_msbar(double x, double bT,double muQ0,double Q0,
                                      Parton flavor,void *Ftildepara,void *Ktildepara,
                                      double rg_transf_Qmax=-1.0 );

}//hso
#endif
