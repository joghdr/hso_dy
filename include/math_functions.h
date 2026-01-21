#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_
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
namespace hso{

  double BesselK0(double x);

  double BesselK(int n,double x);

}//hso
#endif//_MATH_FUNCTIONS_H_
