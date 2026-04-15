#ifndef _FCN_H_
#define _FCN_H_

#include <iosfwd>
#include <string>
#include <vector>
#include "Minuit2/FCNBase.h"

inline constexpr double ErrorOneSigma[] = {
  1.00, //0 parameters
  1.00, //1 parameters
  2.30, //2 parameters
  3.53, //3 parameters
  4.72, //4 parameters
  5.89, //5 parameters
  7.04, //6 parameters
  8.18, //7 parameters
  9.30, //8 parameters
  10.42, //9 parameters
  11.54, //10 parameters
  12.64, //11 parameters
  13.74, //12 parameters
  14.84, //13 parameters
  15.94, //14 parameters
  17.03, //15 parameters
  18.11, //16 parameters
  19.20, //17 parameters
  20.28, //18 parameters
  21.36, //19 parameters
  22.44 //20 parameters
};

inline constexpr double ErrorTwosigma[] = {
  1.00, //0 parameters
  4.00, //1 parameters
  6.18, //2 parameters
  8.02, //3 parameters
  9.72, //4 parameters
  11.31, //5 parameters
  12.85, //6 parameters
  14.34, //7 parameters
  15.79, //8 parameters
  17.21, //9 parameters
  18.61, //10 parameters
  19.99, //11 parameters
  21.35, //12 parameters
  22.69, //13 parameters
  24.03, //14 parameters
  25.34, //15 parameters
  26.65, //16 parameters
  27.95, //17 parameters
  29.24, //18 parameters
  30.52, //19 parameters
  31.80 //20 parameters
};


namespace hso { class Stat;}

namespace ROOT{

  namespace Minuit2{

    extern int fit_iteration_counter;

    class FCN : public FCNBase {

    public:

      FCN(std::vector<hso::Stat*> &stat_pointers,std::vector<std::string> para_names):

      number_of_free_parameters_(1),

      the_ErrorOneSigma_(ErrorOneSigma, ErrorOneSigma + sizeof(ErrorOneSigma) / sizeof(double) ),

      the_ErrorTwosigma_(ErrorTwosigma, ErrorTwosigma + sizeof(ErrorTwosigma) / sizeof(double)){

        fit_iteration_counter=0;

        stat_=&stat_pointers;

        std::vector<std::string> (para_names).swap(para_names_);

        SetConfidenceLevel(1);

      };

      ~FCN(){};


      //methods
      void Info();
      //print/write
      void PrintToScreen(const int &npts,const double &chi2, const std::vector<double> &Parameters)const;

      void WriteToLog(const int &npts,const double &chi2,const std::vector<double> &Parameters)const;
      //calculate the chi2
      void EvalChi2(void * para, double &chi2_value, int &npts,void *pointer) const;

      void SetConfidenceLevel(int n_sigma);
      ///for Minuit
      double virtual Up() const ;

      virtual double operator()(const std::vector<double>& Parameters) const;


      //members
      std::vector<hso::Stat*> *stat_;

      std::vector<std::string> para_names_;

      int number_of_free_parameters_;

      std::vector<double> the_error_def_;

      std::vector<double> the_ErrorOneSigma_;

      std::vector<double> the_ErrorTwosigma_;

      std::ofstream *log_file_;

    };

  }//Minuit2
}//ROOT

#endif//_FCN_H_
