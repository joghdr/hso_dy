
# Hadron Structure Oriented (HSO) approach to TMDs

## Guide

The current version of the code allows the user to

  - perform fits on E288 and E605 fixed-target Drell-Yan data
  - compute Hessian bands for the observables and model eigensets
  - plot some results

the current accuracy of perturbative coefficients is $O(\alpha_s)$.

For the calculation of TMDs in cross sections, some collinear
coefficients, convolutions of perturbative parts with collinear
pdfs, must be computed. In order to speed up the code, these coefficients
are precalculated and stored in LHAPDF grids. Currently, only grids
corresponding to the `MMHT2014nlo68cl` collinear pdfs are made available,
inside the (hidden) directory `.grids/`, which itself contains the following
directories:

  - `MMHT2014nlo68cl/`    (included for convenience, downloadable from the official LHAPDF site)
  - `Apdf_g_Oas1_MMHT2014nlo68cl/`
  - `ApdfOas1_MMHT2014nlo68cl/`
  - `BpdfOas1_MMHT2014nlo68cl/`
  - `fr1Oas1_MMHT2014nlo68cl/`

For the code to run, these directories should be placed in the same
location as other LHAPDF sets. In Ubuntu, when LHAPDF is installed
without the flag `--prefix=/path/for/installation`, it should be something like
`/usr/local/share/LHAPDF/`

### Fits

#### Running from script `scripts/run_fit_fixed_target_DY.sh`

This script summarizes in a practical example the steps
needed to run a fit, including compiling the code from scratch.
Further details are provided below.

#### Preparing the program

The program to perform fits on E288 and E605 data is `programs/fit_fixed_target_DY.c`.
After compiling it, i.e. running the command

  `bash scripts/compile.sh programs/fit_fixed_target_DY.c`

the executable `./exe/fit_fixed_target_DY` will be produced, using the default
TMDPDF and Collins-Soper kernel models in `src/models_fcn-tmd-gaussx1-CS-gauss.cpp`

See [compile](https://github.com/joghdr/hso_dy/blob/main/docs/compile_info.md)
for more information on `scripts/compile.sh`

#### Running the program

The program needs three arguments:

  - kin_file:  the name of the file that indicate kinematical cuts,
                with the same format as `input/kin_drellyan.input`

  - para_file: the name of file that indicate model-parameter settings,
                with the same format as `input/para_hso.input`

  - out_dir_name: the name of the subdirectory where results will be stored,
                  inside `results/`

#### The parameter file

This file should contain all the named parameters defined in the variable
`parameter_names`, in the model file `src/models_fcn-tmd-gaussx1-CS-gauss.cpp`
(or in a custom model file provided by the user). For each parameter,
there should be a line in the parameter file with all of the following:

    PARA_NAME   INIT_PARA_VALUE   IS_FREE   INIT_ERROR   IS_LIMITED   LOWER_LIMIT    UPPER_LIMIT

where

  - `PARA_NAME`: unquoted name of the parameter as defined in model file
  - `INIT_PARA_VALUE`: the starting value of the parameter for Minuit2 (floating point)
  - `IS_FREE`: either 0, to fix the parameter or 1, to let it free during minimization
  - `INIT_ERROR`: initial error parameter for Minuit2 (ignored if IS_FREE = 0)
  - `IS_LIMITED`: either 0, to minimize without limits or 1, to bound the parameter during minimization
  - `LOWER_LIMIT`: the lower bound for the parameter (ignored if either IS_FREE = 0 or IS_LIMITED = 0)
  - `UPPER_LIMIT`: the upper bound for the parameter (ignored if either IS_FREE = 0 or IS_LIMITED = 0)

See the file `input/para_hso.input` for an example corresponding to the
default model file `src/models_fcn-tmd-gaussx1-CS-gauss.cpp`

#### Program`s output

The program will store results in `results/out_dir_name`, inside the following
directory structure:


| subdirectory                     | contains                                   |
| -------------------------------- |:------------------------------------------ |
|                                  |     files with information                 |
|                                  |     on the parameter-covariance            |
|results/out_dir_name/cov/         |     matrix, and corresponding              |
|                                  |     Cholesky matrix, eigenvectors          |
|                                  |     and eigenvalues                        |
|                                  |                                            |
|results/out_dir_name/gnufiles/    |     scripts to plot data included          |
|                                  |     in the fit                             |
|                                  |                                            |
|results/out_dir_name/input/       |     the input files used to run the        |
|                                  |     program, i.e. kin_file and para_file   |
|                                  |                                            |
|results/out_dir_name/min/         |     eigensets according to Minuit`s final  |
|                                  |     output, covariance matrix and a log    |
|                                  |     file with information of each iteration|
|                                  |     in the fit                             |
|                                  |                                            |
|results/out_dir_name/stat/        |     subdirectories with data files used in |
|                                  |     the fit. Each file contains also       |
|                                  |     information of the minimal chi2 point  |
|                                  |     by point.                              |
|                                  |                                            |
|results/out_dir_name/status/      |     final values of the parameters as      |
|                                  |     obtained by Minuit, in the same format |
|                                  |     as `input/para_hso.input`              |

#### Doing a fit with a custom model

By default, the code will use the model in `src/models_fcn-tmd-gaussx1-CS-gauss.cpp`.
The user may provide a different model by modifying a copy of the default file. The
parts to modify are:

  1. the model for the CS kernel, with pQCD constraints:

          double KtildeInputOrder_1(double muQ0, double bT, void *Ktildepara)

      see [2] for a detailed explanation of the pQCD constraints and [3] for the
      default case implemented in the code.


  2. the `core` model [1] for the TMDPDF

          double FtildeCore(double x, double bT, void *Ftildepara)

      This function should not have the QCD constraints from [1] and [2],
      since they are implemented elsewhere in the code.

  3. the names of the parameters of both KtildeInputOrder_1 and FtildeCore

          std::vector<std::string> parameter_names

      the first entry should always be "mF", which is a parameter
      already in use elsewhere in the code

  4. the function minimized by Minuit

          double ROOT::Minuit2::FCN::operator()

  5. the input file for the parameters, in the same format as the sample one
      `input/para_hso.input`, to include all of the names of step 3

Note that the modifications above must keep the consistency in the
order of the parameters. Please, follow the comments marked by the
tag `NOTE` in `src/models_fcn-tmd-gaussx1-CS-gauss.cpp`

After the new model file is ready, the user must compile the code
with the command

    bash src/compile.sh <MAIN_FILE_C> <NEW_MODEL_FILE_CPP>

or

    bash src/compile.sh <MAIN_FILE_C> <NEW_MODEL_FILE_CPP> <EXEC_FILENAME>

Aternatively, the new model filename may be specified in the variable
`models` inside the script `scripts/run_fit_fixed_target_DY.sh`, before
running it


Refs:
[1] Phys.Rev.D 110 (2024) 7, 074016  (DOI: 10.1103/PhysRevD.110.074016)
[2] Phys.Rev.D 106 (2022) 3, 034002  (DOI: 10.1103/PhysRevD.106.034002)


