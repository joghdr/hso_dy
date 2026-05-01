#include <hso/scale_setting.h>
#include <hso/pqcd.h>
#include <cmath>

namespace hso{

  double RGScaleTransformation(double Q0,double Qmax, double bT){

    double output = 0.0;

    output = 1.0-C1/bT/Q0;

    output *= std::exp(-std::pow(bT*Qmax,2.0)); //good

    output = 1.0-output;

    output *= Q0;

    return output;

  }

}//hso
