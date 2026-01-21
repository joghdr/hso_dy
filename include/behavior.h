#ifndef _BEHAVIOR_H_
#define _BEHAVIOR_H_

namespace hso{

  namespace collinear{

    ///NOTE:meant for user
    /// controls the calculation of coefficients A, B in
    /// the pqcd tail of the tmd, direct calculation or
    /// use precalculated LHAPDF grids
    enum class GetCoefficients {FromGrids = 0, ByDirectCalculation = 1};

    extern GetCoefficients ABC_coeff_mode;//defined in src/collinear_factorization.cpp

    void SetCoefficientsMode(GetCoefficients mode);

  }//collinear

}//hso

#endif//_BEHAVIOR_H_
