
# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## Guide

### Intro

The current version of the code allows the user to

  - perform fits on E288 and E605 fixed-target Drell-Yan data
  - compute Hessian bands for the observables and model eigensets
  - plot some results

The current accuracy of perturbative coefficients is $O(\alpha_s)$. See
main publications for details on the theoretical framework [1], [2].

For the calculation of TMD pdfs in cross sections, some collinear
coefficients, convolutions of perturbative parts with collinear
pdfs, must be computed. These coefficients
are precalculated and stored in LHAPDF grids. Currently, only grids
corresponding to the `MMHT2014nlo68cl` collinear pdfs are made available

### Workspace and Settings for Fits

The main workspace to perform fits is the `run/` directory. The following assumes
the user is already in that directory.

The settings of the fit are controlled by two input files:

  - `input/kin_drellyan.input` to set kinematical cuts
  - `input/para_hso.input` for parameter settings

The user can control the behaviour of the fitter by directly modifying these files.

### Running the Fitter and Producing Hessian bands
*NOTE: Before running the programs, the user must build the executables, see [build](build_info.md)*

The programs are run through helper scripts

  - `scripts/run_fitter_docker.sh` - runs the fit on E288 and/or E605 data
  - `scripts/run_hessian_docker.sh` - takes results from a completed fit and computes the Hessian bands on the data

The typical workflow would start by running the fit:

  ```bash
  #run the fit and store results in 'results/FIT_DIR_NAME'
  bash scripts/run_fitter_docker.sh FIT_DIR_NAME
  ```
Once completed, it is possible to generate the Hessian bands by running:
  ```bash
  # calculate Hessian bands given for results/FIT_DIR_NAME and store them in 'results/HESSIAN_DIR_NAME'
  bash scripts/run_hessian_docker.sh results/FIT_DIR_NAME HESSIAN_DIR_NAME
  ```
*Note: the user passes a directory name which will always be stored inside `results/`, the directory must not exists.*

### Visualizing Results with `Gnuplot` (requires gnuplot 6.0 or later)
Once these fits and Hessian bands are computed, a plot to compare fit and data can be generated from
a the convenience commands defined inside `scripts/commands.sh`, `plotE288` and `plotE605`:

  ```bash
  #need to load the commands only once per session
  source scripts/commands.sh
  #plot the data, central theory lines and bands stored in 'results/HESSIAN_DIR_NAME'
  plotE288 results/HESSIAN_DIR_NAME

  plotE605 results/HESSIAN_DIR_NAME
  ```
The plot commands print the full path of the `pdf` file generated, which will be located inside `results/HESSIAN_DIR_NAME/plots/`.
The gnuplot script file used to generate the plot, with extension `.gnu`, is stored in the same directory.

*Note: The plot commands generate the gnuplot script only if not already present. This is useful if the user wants to fine-tune the plots. Just modify the `.gnu` file and rerun the corresponding plot command.*

#### Output of the Fitter (`scripts/run_fitter_docker.sh`)

The program will store results inside the directory `results/`. The user specifies
`FIT_DIR_NAME` and the fitter will produce the following:

| subdirectory                     | contains                                   |
| -------------------------------- |:------------------------------------------ |
|`results/FIT_DIR_NAME/cov/`       |     files with information on the parameter-covariance matrix, and corresponding Cholesky matrix, eigenvectors and eigenvalues
|`results/FIT_DIR_NAME/input/`     |     copies of the input files used to run the program
|`results/FIT_DIR_NAME/min/`       |     eigensets according to Minuit`s final output, log file with information of each iteration, Minuit's output
|`results/FIT_DIR_NAME/stat/`      |     subdirectories with data files used in the fit. Each file contains also information of the minimal chi2 point by point.
|`results/FIT_DIR_NAME/status/`    |     final values of the parameters as obtained by Minuit, in the same format as `input/para_hso.input`


#### Output of the Hessian Bands (`scripts/run_hessian_docker.sh`)

When running the script, the user specifies the **path** of a previous fit,
`results/FIT_DIR_NAME`, and the name of the output directory `HESSIAN_DIR_NAME`. The program produces a series of subdirectories:

| subdirectory                     | contains                                   |
| -------------------------------- |:------------------------------------------ |
|`results/HESSIAN_DIR_NAME/input/` |     a copy of the input file `kin_drellyan.input`, used to run the program
|`results/HESSIAN_DIR_NAME/stat/`  |     subdirectories with data files, containing data, errors, best fit central lines and Hessian uncertainties. Each file contains also information of the minimal chi2 point by point.
|`results/HESSIAN_DIR_NAME/status/`|     parameter eigensets, a log file and a copy of the parameter file with best-fit values, in the same format as `input/para_hso.input`



Refs:\
[1]: Phys.Rev.D 110 (2024) 7, 074016  (DOI: 10.1103/PhysRevD.110.074016)\
[2]: Phys.Rev.D 106 (2022) 3, 034002  (DOI: 10.1103/PhysRevD.106.034002)

