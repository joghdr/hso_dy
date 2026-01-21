#include "pqcd.h"
#include "LHAPDF/LHAPDF.h"

namespace hso{

  namespace collinear{

    extern LHAPDF::PDF* f_MSbar;

  }

  double AlphaStrong(double scale){

    return collinear::f_MSbar->alphasQ(scale);

  }

  double as(double scale){

    return AlphaStrong( scale)/4.0/Pi;

  }//collinear

}//hso
