#include "objects_stat.h"
#include "objects_data.h"
#include "objects_theory.h"
#include "stat_class.h"


namespace hso{

  hso::StatNMin chi2_e288 ("E288", &observable_drell_yan_1, e288, 0.00, 0.25);

  hso::StatNMin chi2_e605 ("E605", &observable_drell_yan_4_xFavg_qTavg, e605, 0.05, 0.15);

  hso::StatNMin chi2_cdf_I ("CDF_I", &observable_drell_yan_Z0_1, cdfI_Z0pt_1999, 0, 0.039);

  hso::StatNMin chi2_cdf_II ("CDF_II", &observable_drell_yan_Z0_1, cdfII_Z0pt_2012, 0, 0.058);//missing pt by pt systematic. fix for fits

  hso::StatNMin chi2_d0_I ("D0_I", &observable_drell_yan_Z0_1, d0I_Z0pt_1999, 0, 0.044);

  std::vector<hso::Stat*> chi2_total {

    &chi2_e288,

    &chi2_e605

  };

}//hso
