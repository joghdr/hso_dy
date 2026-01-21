
# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## Description

Language: C++ (Tested with gcc compilers, i.e. Linux)
The code allows to reproduce results analogous to those published in:

  Phys.Rev.D 110 (2024) 7, 074016  (DOI: 10.1103/PhysRevD.110.074016)

Other relevant publications are:

    Phys.Rev.D 106 (2022) 3, 034002  (DOI: 10.1103/PhysRevD.106.034002)
    Phys.Rev.D 107 (2023) 9, 094029  (DOI: 10.1103/PhysRevD.107.094029)

Some bash scripts are included for certain tasks, like compiling and running the
sample programs.

## Dependencies (see 'doc/dependencies_info.txt' for further information)

The code relies on several external libraries and packages, which the
user must install

    - i)   gnu compiler collection, in particular g++ (g++ 13.3.0 or later)
    - ii)  gnu scientific libraries (libgsl-dev 2.7.1 or later)
    - iii) boost libraries (libboost-all-dev 1.83.0.1 or later)
    - iv)  gnuplot (version 6.0 or later)
    - v)   cuba integration libraries (version 4.2.2)
    - vi)  lhapdf (version 6.5.4 and later)
    - vii) Minuit2 (standalone, version 5.28.00 or later)

The code has been developed in Ubuntu 24.04 (noble), but it should work in other
Linux distributions that use gcc compilers.

## Compiling the HSO code (see 'doc/compile_info.txt' for further information)

The code has the following directory structure:

  src/         source files and binaries (after first compilation)
  include/     header files
  programs/    sample programs
  scripts/     bash scripts for different tasks
  exe/         executables (after compilation)
  input/       sample input files for parameters and kinematical cuts

The code can be compiled by simply running the bash script 'compile.sh',
which requires at least one argument, the name of a .c main file. For instance,
running in the terminal the command

    bash scripts/compile.sh programs/fit_fixed_target_DY.c

will create all binaries inside 'src/' and the main program 'exe/fit_fixed_target_DY.c'.

## Use cases (see 'doc/guide.txt' for further information)

The current version of the code allows the user to

    - perform fits on E288 and E605 fixed-target Drell-Yan data
    - compute tmd eigensets and Hessian bands for observables
    - plot some results

The current accuracy of perturbative coefficients is O(alpha_s)




