#ifndef _STAT_CLASS_H_
#define _STAT_CLASS_H_

#include "data_class.h"
#include "theory_class.h"
#include <string>
#include <vector>

namespace hso{

  extern bool store_values_stat;

  class Stat {

  public:

    Stat(std::string name_in, Theory* theory_pointer, std::vector<Data*> &data_pointers):

         length_(0), err_values_(0), stat_values_(0), name_(name_in) {

      theory_=theory_pointer;

      data_ = &data_pointers;

      rel_err_sys_pp_=0.0;

      LoadData();

      fix_norm_=false;

      penalties_exist_=false;

      hessian_error_present_=false;

    };

    Stat(std::string name_in, Theory* theory_pointer, std::vector<Data*> &data_pointers, double rel_err_sys_pp_in):

         rel_err_sys_pp_(rel_err_sys_pp_in), length_(0), err_values_(0), stat_values_(0), name_(name_in) {

      theory_=theory_pointer;

      data_ = &data_pointers;

      LoadData();

      fix_norm_=false;

      penalties_exist_=false;

      hessian_error_present_=false;

    };

    Stat(){};

    ~Stat(){};

    //methods
    bool CheckStatFilled(int i);

    void GetBounds(int i,std::vector<double> &bounds);

    double GetTotalValue(int i);

    double GetTotalPoints(int i);

    double GetTotalValue();

    double GetTotalPoints();

    virtual double LoadError(Data* data,int i);

    void UpdateIsActive();

    void LoadData();

    void SetRange(std::string varname, double min, double max);

    void SetNumOfEigen(int numofeigen);

    void EvalHessianError();

    virtual double Eval(double meas,double error,double theory,void *pointer);

    virtual double EvalPenalties(void *pointer);

    virtual double operator()(void *para,void *pointer,bool store_values=store_values_stat);

    virtual bool Write(std::string fname_out,int i);

    //members
    bool fix_norm_;

    bool penalties_exist_;

    bool hessian_error_present_;

    double rel_err_sys_pp_;

    Theory *theory_;

    std::vector<Data*> *data_;

    std::string gnuscript_;

    int length_;

    int length_active_;

    std::vector< bool> point_is_active_;

    std::vector<double> meas_values_;

    std::vector<double> err_values_;

    std::vector<double> stat_values_;

    std::string name_;

  };//Stat


  class StatN : public Stat {

  public:

    StatN(std::string name_in, Theory* theory_pointer, std::vector<Data*> &data_pointers, double rel_err_sys_pp_in, double delta_N__in):

          Stat(name_in, theory_pointer, data_pointers, rel_err_sys_pp_in), delta_N_(delta_N__in) {

      for(auto set: *data_) {

        set->normalize_=true;

        std::vector<double>(set->length_,1.0).swap(set->norm_values_);

      }

      penalties_exist_=true;

    };

    StatN(){};

    ~StatN(){};
    //methods
    virtual double Eval(double meas,double error,double theory,void *pointer);

    virtual double EvalPenalties(void *pointer);
    //members
    double delta_N_;
  };//StatN


  class StatNMin : public Stat {

  public:

    StatNMin(std::string name_in,Theory* theory_pointer,std::vector<Data*> &data_pointers,double rel_err_sys_pp_in,double delta_N__in):

             Stat(name_in,theory_pointer,data_pointers,rel_err_sys_pp_in),delta_N_(delta_N__in) {

      for(auto set: *data_) {

        set->normalize_=true;

        std::vector<double>(set->length_,1.0).swap(set->norm_values_);

      }

      penalties_exist_=true;

    };

    StatNMin(){};

    ~StatNMin(){};
    //members
    double delta_N_;
    //no need for Eval or Eval_penalties
    virtual double operator()(void *para,void *pointer,bool store_values=store_values_stat);

  };//StatNMin

}//hso

#endif //_STAT_CLASS_H_



