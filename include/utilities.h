#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "data_class.h"
#include "stat_class.h"
#include "FCN.h"
#include "Minuit2/FunctionMinimum.h"
#include <vector>
#include <string>

namespace hso{

  namespace utils{

    extern bool verbose;

    int GetFreeParaNumber(ROOT::Minuit2::MnUserParameters &upar);

    bool ReadSquareMatrix(std::string filename, std::string matrix_tag, std::vector<int> &row_indices,

                          std::vector<int> &col_indices, std::vector<double> &matrix);

    void WriteCovariance(ROOT::Minuit2::FunctionMinimum* min);

    void WriteCovarianceEigen(std::string covariance_filename);

    void ReadVector(std::string filename, std::string vector_tag, std::vector<double> &output);

    void TransformParameters(const std::vector<double> &eve ,const std::vector<double> &parameters,

                             std::vector<double> &transformed_parameters, const std::vector<int> &free_index);

    double DistanceToBoundary (double para_value, void *Perr);

    double Scan(double init_shift_factor,void *Perr);

    void ReadEigensets(std::string eigen_filename, std::vector<std::string> &eigen_para_names,

                       std::vector<double> &eigen_sets, int &numofeigen, int &numofpara);

    void GetEigenset(const std::vector<double> &eigen_sets,const int &numofeigen,

                     const int &numofpara,const int &i, std::vector<double> &eigen_set_i);

    void FilesInDir(const std::string dir,std::vector<std::string> &filenames);

    void FilesInDir(std::string dir,std::string substring,std::vector<std::string > &filenames);

    std::string FileInDirUnique(std::string dir, std::string substring);

  }//utils

  //NOTE: user level functions


  void InitProgram(int argc, char *argv[] ,

                   std::vector<std::string> expected_args={"kin_file","para_file","out_dir_name"},

                   std::vector<std::string> optional_args={} );

  void InitCollinear();

  void SetRange(std::vector<hso::Data*> data_pointers , std::string var_name,

                double var_min, double var_max);

  void InitKinematics();

  void SetMinuitParameters( ROOT::Minuit2::MnUserParameters & upar,

                            std::vector<std::string> parameter_names);

  ROOT::Minuit2::FunctionMinimum *Minimize(ROOT::Minuit2::FCN &theFCN,

                                           ROOT::Minuit2::MnUserParameters & upar);

  void Write(hso::Stat* stat,std::string subdir="stat");

  void Write(std::vector<hso::Stat*> stat_pointers, std::string subdir="stat");

  void Plot(hso::Stat* stat,std::string x, std::string y, std::string err="stat+");

  void Plot(std::vector<hso::Stat*> stat_pointers, std::string x, std::string y, std::string err="stat+");

  void Errors(ROOT::Minuit2::FCN &theFCN, ROOT::Minuit2::FunctionMinimum &min);

  void InitHessianBands( int argc, char *argv[] );

  void HessianBands(ROOT::Minuit2::FCN &theFCN, ROOT::Minuit2::MnUserParameters & upar);

}//hso

#endif//_UTILITIES_H_

