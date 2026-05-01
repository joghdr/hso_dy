
# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## Description

Language: C++(20) (Tested with gcc compilers, i.e. Linux)
The code allows to reproduce results analogous to those published in:

  - [Phys.Rev.D 110 (2024) 7, 074016](https://inspirehep.net/literature/2751355)

Other relevant publications on the HSO approach are:

  - [Phys.Rev.D 106 (2022) 3, 034002](https://inspirehep.net/literature/2080521)
  - [Phys.Rev.D 107 (2023) 9, 094029](https://inspirehep.net/literature/2640018)

## Build Requirements

The build system requires`CMake(3.28)`, `Ninja(1.11)` and `Docker(29.4)`, which the user must pre-install.

## Building the Executables

A containerized build environment is provided via Docker for portability (see [build](hso_engine/docs/build_info.md)). This enables a simple one-step build.

Advanced users and developers may want to take a look at the `CMakePresets.json` for available presets, `CMakeLists.txt` for targets/tests and the information on dependencies below.


## Dependencies (see [dependencies](hso_engine/docs/dependencies_info.md) for details)
*NOTE: when using the containerized build, all dependencies are automatically handled*

The code relies on several external libraries and packages, which the user must install

  1. gnu compiler collection, in particular g++ (g++ 13.3.0 or later)
  2. gnu scientific libraries (libgsl-dev 2.7.1 or later)
  3. cuba integration libraries (version 4.2.2)
  4. lhapdf (version 6.5.4 or later)
  5. Minuit2 (standalone, version 5.28.00 or later)
  6. gnuplot (**optional** for plotting, version 6.0 or later)

The code has been developed in Ubuntu 24.04 (noble), but it should work in other
Linux distributions that use gcc compilers.

## Use Cases & Running the Programs (see [guide](hso_engine/docs/guide.md) for details)

The current version of the code allows the user to

  - perform fits on E288 and E605 fixed-target Drell-Yan data
  - compute tmd eigensets and Hessian bands for observables
  - plot some results

The current accuracy of perturbative coefficients is $O(\alpha_s)$

A bash script is provided to run the executables inside the Docker container.




