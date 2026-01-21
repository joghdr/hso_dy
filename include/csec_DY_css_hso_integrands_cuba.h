#ifndef _CSEC_RG_IMPROVED_INTEGRANDS_H_
#define _CSEC_RG_IMPROVED_INTEGRANDS_H_

namespace hso{

  namespace csec_no_rg_improved {
    ///@prototype: fixed target E288 experiment.
    ///@units: fb * GeV^-2
    int integrand_drell_yan_1(

      const int *ndim, const double xx[],

      const int *ncomp, double ff[], void *P

    );

    int integrand_drell_yan_4_xFavg_qTavg(

      const int *ndim, const double xx[],

      const int *ncomp, double ff[], void *P

    );

  }//csec_no_rg_improved

  namespace csec_rg_improved {

    int integrand_drell_yan_Z0_1(

      const int *ndim, const double xx[],

      const int *ncomp, double ff[], void *P

    );

    ///@prototype: fixed target E288 experiment.
    ///@units: fb * GeV^-2
    int integrand_drell_yan_1(

      const int *ndim, const double xx[],

      const int *ncomp, double ff[], void *P

    );

    ///@prototype: fixed target E605 experiment.
    ///@units: pb * GeV^-2
    int integrand_drell_yan_4_xFavg_qTavg(

      const int *ndim, const double xx[],

      const int *ncomp, double ff[], void *P

    );

  }//csec_rg_improved

}//hso

#endif//_CSEC_RG_IMPROVED_INTEGRANDS_H_
