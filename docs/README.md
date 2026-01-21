
# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## Description

Language: C++ (Tested with gcc compilers, i.e. Linux)
The code allows to reproduce results analogous to those published in:

  - [Phys.Rev.D 110 (2024) 7, 074016](https://inspirehep.net/literature/2751355)

Other relevant publications are on the HSO approach are:

  - [Phys.Rev.D 106 (2022) 3, 034002](https://inspirehep.net/literature/2080521)
  - [Phys.Rev.D 107 (2023) 9, 094029](https://inspirehep.net/literature/2640018)

Some bash scripts are included for certain tasks, like compiling and running the
sample programs.

## Dependencies (see [dependencies](https://github.com/joghdr/hso_dy/blob/main/docs/dependencies_info.md) for details)

The code relies on several external libraries and packages, which the user must install

  1. gnu compiler collection, in particular g++ (g++ 13.3.0 or later)
  2. gnu scientific libraries (libgsl-dev 2.7.1 or later)
  3. boost libraries (libboost-all-dev 1.83.0.1 or later)
  4. gnuplot (version 6.0 or later)
  5. cuba integration libraries (version 4.2.2)
  6. lhapdf (version 6.5.4 and later)
  7. Minuit2 (standalone, version 5.28.00 or later)

The code has been developed in Ubuntu 24.04 (noble), but it should work in other
Linux distributions that use gcc compilers.

## Compiling the HSO code (see [compile](https://github.com/joghdr/hso_dy/blob/main/docs/compile_info.md) for details)

The code has the following directory structure:

  - `src/`         source files and binaries (after first compilation)
  - `include/`     header files
  - `programs/`    sample programs
  - `scripts/`     bash scripts for different tasks
  - `exe/`         executables (after compilation)
  - `input/`       sample input files for parameters and kinematical cuts

The code can be compiled by simply running the bash script `scripts/compile.sh`,
which requires at least one argument, the name of a .c main file. For instance,
running in the terminal the command

  `bash scripts/compile.sh programs/fit_fixed_target_DY.c`

will create all binaries inside `src/` and the main program `exe/fit_fixed_target_DY.c`.

## Use cases (see 'doc/guide.txt' for details)

The current version of the code allows the user to

  - perform fits on E288 and E605 fixed-target Drell-Yan data
  - compute tmd eigensets and Hessian bands for observables
  - plot some results

The current accuracy of perturbative coefficients is O(alpha_s)




