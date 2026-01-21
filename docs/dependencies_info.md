
# Hadron Structure Oriented (HSO) approach to TMDs  ##############

## Dependencies

The code relies on several external libraries and packages, which the
user must install (see installation summary guide below)

  1. gnu compiler collection, in particular g++ (g++ 13.3.0 or later)
  2. gnu scientific libraries (libgsl-dev 2.7.1 or later)
  3. boost libraries (libboost-all-dev 1.83.0.1 or later)
  4. gnuplot (version 6.0 or later)
  5. cuba integration libraries (version 4.2.2)
  6. lhapdf (version 6.5.4 or later)
  7. Minuit2 (standalone, version 5.28.00 or later)

The code has been developed in Ubuntu 24.04 (noble), but it should work in other
Linux distributions that support gcc compilers version 13.3.0 or later

## Installation guide for dependencies

### Things to install using `apt`

  If not already present, libraries 1-4 can be installed from
  the terminal. In Ubuntu (or other debian system), run the commands:

  ```
  sudo apt install gcc
  sudo apt install g++
  sudo apt install libgsl-dev
  sudo apt install libboost-all-dev
  sudo apt install gnuplot
  ```
### Installing `Cuba`

  For full instructions how to install Cuba, see documentation:

  - website (source code):
          https://feynarts.de/cuba/

  - publication (installation instructions):
          Comput.Phys.Commun. 168 (2005) 78-95 (e-Print: https://arxiv.org/abs/hep-ph/0404043)

  - other publications:
          J.Phys.Conf.Ser. 608 (2015) 1, 012066  Contribution to: ACAT 2014 (e-Print: https://arxiv.org/abs/1408.6373)

  Installation summary (It is recommended to check full instructions in publication above):

  - download tarball Cuba-4.2.2.tar.gz from https://feynarts.de/cuba/
  - go to the directory where you want to build the package
  - run the commands:
    ```
    tar -xzf PATH_TO_TARBALL/Cuba-4.2.2.tar.gz
    cd Cuba-4.2.2`
    ./configure
    make
    sudo make install
    ```

  vi) LHAPDF

    For full instructions how to install LHAPDF, see documentation:

      website:
            https://www.lhapdf.org/

      source:
            https://lhapdf.hepforge.org/downloads/

      main publication:
            Eur.Phys.J.C 75 (2015) 132 ( e-Print: https://arxiv.org/abs/1412.7420)

      Installation summary (It is recommended to check full instructions from website above):

        - Download tarball from https://lhapdf.hepforge.org/downloads/
        - go to the directory where you want to build the package
        - run the commands (with appropriate version)
          tar -xzf PATH_TO_TARBALL/LHAPDF-6.5.4.tar.gz
          cd  LHAPDF-6.5.4
          ./configure --prefix=/path/for/installation
          make
          make install
        - Note that --prefix=/path/for/installation above is only necessary to choose
          an installation path other than standard ones (see full LHAPDF documentation)
        - Take note of the path where the different PDF grids are stored

  vii) Minuit2 (standalone version)

    For source files and instructions to install Minuit2, see

      repo:
            https://github.com/GooFit/Minuit2

