#ifndef _GLS_SETTINGS_H_
#define _GLS_SETTINGS_H_

namespace hso{

  namespace gsl_default{

    inline constexpr double kGSLEpsAbs = 1E-20;

    inline constexpr double kGSLEpsRel = 1E-4;

    inline constexpr int kGSLLimit = 100;

    inline constexpr double kGSLEpsAbsFourierTransform = 1E-30;

    inline constexpr double kGSLEpsRelFourierTransform = 1E-5;

    inline constexpr int kGSLLimitFourierTransform = 1000;

    inline constexpr int kGSLKey = 2;

    inline constexpr int kGSLWIntervals = 200;

    inline constexpr int kGSLWIntervalsFourierTransform = 1000;

    inline constexpr int kGSLFixedN = 200;

  }//gsl_default

}//hso

#endif//_GLS_SETTINGS_H_
