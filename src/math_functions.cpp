#include <hso/math_functions.h>
#include <cmath>

namespace hso{

  double BesselK0(double x){

    long double output=0.0;

    output=std::cyl_bessel_k(0, x);

    return output;

  }

  double BesselK(int n,double x){

    long double output=0.0;

    output=std::cyl_bessel_k(n, x);

    return output;

  }

}//hso
