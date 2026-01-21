#ifndef _CUBA_SETTINGS_H_
#define _CUBA_SETTINGS_H_

namespace hso{
  //NOTE not defined here:
  // kCubaUserData = NULL;
  // kCubaStateFile = NULL;
  // kCubaSpin = NULL;
  namespace cuba_default {

    inline constexpr int kCubaNDim = 3;

    inline constexpr int kCubaNComp = 1;

    inline constexpr int kCubaNVec = 1;

    inline constexpr double kCubaEpsRel = 1e-4;

    inline constexpr double kCubaEpsAbs = 1e-30;

    inline constexpr int CubaVerbose = 0;

    inline constexpr int kCubaLast = 4;

    inline constexpr int kCubaSeed = 0;

    inline constexpr int kCubaMinEval = 0;

    inline constexpr int kCubaMaxEval = 500000;

    inline constexpr int kCubaNStart = 1000;

    inline constexpr int kCubaNIncrease = 500;

    inline constexpr int kCubaNBatch = 1000;

    inline constexpr int kCubaGridNo = 0;

    inline constexpr int kCubaNNew = 1000;

    inline constexpr int kCubaNMin = 2;

    inline constexpr double kCubaFlatness = 25.;

    inline constexpr int kCubaKey1 = 47;

    inline constexpr int kCubaKey2 = 1;

    inline constexpr int kCubaKey3 = 1;

    inline constexpr int kCubaMaxPass = 5;

    inline constexpr double kCubaBorder = 0.;

    inline constexpr double kCubaMaxChisq = 10.;

    inline constexpr double kCubaMinDeviation = .25;

    inline constexpr int kCubaNGiven = 0;

    inline constexpr int kCubaLdxGiven = 1;

    inline constexpr int kCubaNExtra = 0;

    inline constexpr int kCubaKey = 0;

  }//cuba_default

}//hso
#endif//_CUBA_SETTINGS_H_
