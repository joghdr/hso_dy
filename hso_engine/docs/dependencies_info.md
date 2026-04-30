
# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## User vs Developer
Details to build the executables in a host machine are provided here, but they are meant for developers or advanced users that require to modify the source code, e.g. define custom models. Otherwise, a user may skip these steps and simply build (see [build](https://github.com/joghdr/hso_dy/blob/main/docs/build_info.md)) and run executables (see [guide](https://github.com/joghdr/hso_dy/blob/main/docs/guide.md)) using a
the containerized Docker environment.

## Build Requirements

The build system requires`CMake(3.28)`, `Ninja(1.11)` and `Docker(29.4)`, which the user must pre-install.


## Dependencies
The code relies on several external libraries and packages. In order to build
the code locally, the user must first install:

  1. gnu compiler collection, in particular g++ (g++ 13.3.0 or later)
  2. gnu scientific libraries (libgsl-dev 2.7.1 or later)
  3. cuba integration libraries (version 4.2.2)
  4. lhapdf (version 6.5.4 or later)
  5. Minuit2 (standalone, version 5.28.00 or later)
  6. gnuplot (**optional** for plotting, version 6.0 or later)
*see installation summary guide below*


The code has been developed in Ubuntu 24.04 (noble), but it should work in other
Linux distributions that support gcc compilers version 13.3.0 or later

## Installation guide for dependencies

### Things to install using `apt`

  If not already present, libraries 1-4 can be installed from
  the terminal. In Ubuntu (or other debian system), run the commands:

  ```bash
  sudo apt update && sudo apt install gcc g++ libgsl-dev gnuplot -y
  ```

### Installing `Cuba`

  For full instructions how to install Cuba, see documentation:

  - website (source code): https://feynarts.de/cuba/

  - publication (installation instructions):\
          Comput.Phys.Commun. 168 (2005) 78-95\
          (e-Print: https://arxiv.org/abs/hep-ph/0404043)

  - other publications:\
          J.Phys.Conf.Ser. 608 (2015) 1, 012066  Contribution to: ACAT 2014\
          (e-Print: https://arxiv.org/abs/1408.6373)

#### Installation summary (It is recommended to check full instructions in publication above):

  - download tarball `Cuba-4.2.2.tar.gz` from https://feynarts.de/cuba/
  - go to the directory where you want to build the package
  - run the commands:
    ```
    tar -xzf <PATH_TO_TARBALL>/Cuba-4.2.2.tar.gz
    cd Cuba-4.2.2
    ./configure
    make
    sudo make install
    ```

### Installing `LHAPDF`

  For full instructions how to install LHAPDF, see documentation:

  - website: https://www.lhapdf.org/

  - source: https://lhapdf.hepforge.org/downloads/

  - main publication:\
        Eur.Phys.J.C 75 (2015) 132\
        (e-Print: https://arxiv.org/abs/1412.7420)

#### Installation summary (It is recommended to check full instructions from website above):

  - Download tarball from https://lhapdf.hepforge.org/downloads/
  - go to the directory where you want to build the package
  - run the commands (with appropriate version)
    ```
    tar -xzf <PATH_TO_TARBALL>/LHAPDF-6.5.4.tar.gz
    cd  LHAPDF-6.5.4
    ./configure --prefix=<PATH_FOR_INSTALLATION>
    make
    make install
    ```
  - Note that `--prefix=<PATH_FOR_INSTALLATION>` above is optional. To install
    in standard path, there is no need to use the flag (see full LHAPDF documentation)
  - Take note of the path where the different PDF grids should be stored

*NOTE: if LHAPDF is install in a non standard path, CMake needs to be aware of this. The best way to do it is by adding
the entry "CMAKE_PREFIX_PATH": "<PATH_FOR_INSTALLATION>" to the configure preset named "release", located in `CMakePresets.json`*

### Installing Minuit2 (standalone version)

For source files and instructions to install Minuit2, see

  - repo: https://github.com/GooFit/Minuit2

**Important**: Cuba is a fast integrator but it doesn't play along well with Minuit's multithreading. If installation of Minuit2 is done with the `-Dminuit2_omp=OFF` flag, the executables should run correctly. Otherwise, the user must set the environment's number of threads to 1, and cuba's to (about) 5, if available:

  ```bash
    export OMP_NUM_THREADS=1
    export CUBACORES=5
  ```
Running with about 5 CUBACORES reached better performance at testing.






