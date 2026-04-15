#include "FCN.h"
#include "stat_class.h"
#include <vector>
#include <iostream>
#include <fstream>

extern double Q0,Qmax;

namespace ROOT{

  namespace Minuit2{

    int fit_iteration_counter = 0;

    void FCN::Info(){

      std::cout<<__FILE__<<": "<<__LINE__<<": "<<"fit_iteration_counter"<<"="<<fit_iteration_counter<<"\n";

      std::cout<<__FILE__<<": "<<__LINE__<<": "<<"stat size"<<"="<<stat_->size()<<"\n";

    }
    //print/write methods
    void FCN::PrintToScreen(const int &npts,const double &chi2, const std::vector<double> &Parameters)const{

      std::cout<<fit_iteration_counter<<"\t";

      for(auto para: Parameters)

        std::cout<<para<<"\t";

      std::cout<<npts<<"\t";

      std::cout<<chi2<<"\t";

      std::cout<<chi2/npts<<"\t";

      std::cout<<std::endl;

    }

    void FCN::WriteToLog(const int &npts,const double &chi2,const std::vector<double> &Parameters)const{

      //write to log file
      *log_file_<<fit_iteration_counter<<"\t";

      for(auto para: Parameters)

        *log_file_<<para<<"\t";

      *log_file_<<npts<<"\t";

      *log_file_<<chi2<<"\t";

      *log_file_<<chi2/npts<<"\t";

      *log_file_<<std::endl;

    }

    void FCN::SetConfidenceLevel(int n_sigma){

      if (n_sigma==2) {

        the_error_def_=the_ErrorTwosigma_;

        std::cout<<"\n# setting 2-sigma delchi2:\n";

      }

      else {

        the_error_def_=the_ErrorOneSigma_;

        std::cout<<"\n# setting 1-sigma delchi2:\n";

      }

      std::cout<<"#"<<the_error_def_[1]<<"\t"<<"#"<<the_error_def_[2]<<"\t"<<"#"

               <<the_error_def_[3]<<"\n";

    }

    void FCN::EvalChi2(void * para, double &chi2_value, int &npts,void *pointer) const{//WARNING: it does not reset 'chi2_value' or 'npts'

      for(auto entry: *stat_){

        chi2_value += entry->operator()(para,pointer);

        npts += entry->GetTotalPoints();

      }

    }
    ///for Minuit
    double FCN::Up() const {

      double output = 0.0;

      output = the_error_def_[number_of_free_parameters_];

      return output;

    }

  }//Minuit2

}//ROOT
