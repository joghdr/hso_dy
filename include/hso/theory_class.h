#ifndef _THEORY_CLASS_H_
#define _THEORY_CLASS_H_

#include "cuba.h"
#include <initializer_list>
#include <string>
#include <vector>


typedef int (*CubaFunction) (

  const int *ndim, const cubareal xx[],

  const int *ncomp, cubareal ff[], void *P);


namespace hso{

  class Data;

  extern bool store_values_theory;

  extern int eigen_index;

  enum class VariableType {Value, Integration, BinAvg, Undetermined};

  class Theory {

  public:

    Theory();

    Theory(CubaFunction integrand, std::initializer_list<std::string> varnames_in,

           std::initializer_list<VariableType> behave_in):

           var_names_(varnames_in),var_behave_(behave_in) {

      cuba_integrand_ = integrand;

      print_warning_undetermined_=0;

      for(auto entry: var_behave_)

        if(entry == VariableType::Undetermined )

          print_warning_undetermined_++;

    }

    ~Theory(){};

    //methods
    void Info();

    bool GetVarValues(Data &data,int i, std::vector<double> &values,std::vector<double> &min,std::vector<double> &max,std::vector<double> &binavg);

    double Eval(Data &data,int i,void *para);

    std::vector<double> operator()(Data &data,void *para,bool store_values=store_values_theory,int eigen_i=eigen_index);

    bool Write(Data &data,std::string fname_out,bool alldata);

    //members
    int print_warning_undetermined_;

    CubaFunction cuba_integrand_ ;

    std::string gnu_script_;

    int length_;

    std::vector<std::string> var_names_;

    std::vector<VariableType> var_behave_;

  };

}//hso

#endif//_THEORY_CLASS_H_

