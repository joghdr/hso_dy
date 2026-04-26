#include <hso/math_functions.h>
// #include <boost/math/special_functions/bessel.hpp>
#include <cmath>

namespace hso{
  // //bessel function
  // typedef boost::math::policies::policy<
  // // domain_error<errno_on_error>,
  // // pole_error<errno_on_error>,
  // // evaluation_error<errno_on_error>,
  // boost::math::policies::overflow_error<boost::math::policies::ignore_error>
  // > besselK_policy;
  //bessel function
  // typedef boost::math::policies::policy<
  // domain_error<errno_on_error>,
  // promote_double<false>,
  // digits10<3>,
  // boost::math::policies::overflow_error<boost::math::policies::ignore_error>
  // > besselK_policy;
  double BesselK0(double x){

    long double output=0.0;

    output=std::cyl_bessel_k(0, x/*,besselK_policy()*/);

    return output;

  }

  double BesselK(int n,double x){

    long double output=0.0;

    output=std::cyl_bessel_k(n, x/*,besselK_policy()*/);

    return output;

  }

}//hso
