#include <hso/objects_stat.h>
#include <hso/objects_data.h>
#include <hso/objects_theory.h>
#include <hso/stat_class.h>


namespace hso{

  hso::StatNMin chi2_e288 ("E288", &observable_drell_yan_1, e288, 0.00, 0.25);

  hso::StatNMin chi2_e605 ("E605", &observable_drell_yan_4_xFavg_qTavg, e605, 0.05, 0.15);

  std::vector<hso::Stat*> chi2_total {

    &chi2_e288,

    &chi2_e605

  };

}//hso
