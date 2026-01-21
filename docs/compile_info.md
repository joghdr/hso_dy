
# Hadron Structure Oriented (HSO) approach to TMDs

## Compiling the code

The code has the following directory structure:

  - `src/`         source files and binaries (after first compilation)
  - `include/`     header files
  - `programs/`    sample programs
  - `scripts/`     bash scripts for different tasks
  - `exe/`         executables (after compilation)
  - `input/`       sample input files for parameters and kinematical cuts

The code can be compiled by simply running the bash script `scripts/compile.sh`,
which will produce

  - `.o` binary files, stored inside `src/`
  - executable, stored in `exe/`

Binary files are compiled from their respective `.cpp` source files
only if not already present inside `src/`. For instance, running
the script for the first time with the command

  `bash scripts/compile.sh programs/fit_fixed_target_DY.c`

will produce both binaries and executable. A scecond run of the command,
will only produce the executable.

### Functionalities of `src/compile.sh`

The script takes either zero, one, two or three arguments.

  - *mode 0*\
    Running the command\
    `bash scripts/compile.sh`\
    will print a message with the different arguments that the script can take.

  - *mode 1*\
    Running the command\
    `bash src/compile.sh <MAIN_FILE_C>`\
    will compile the program  `<MAIN_FILE_C>` (extension `.c`) using the
    default models for TMDPDF and Collins-Soper kernel, contained in
    `src/models_fcn-tmd-gaussx1-CS-gauss.cpp`, producing an executable
    named after `<MAIN_FILE_C>` but without the extension `.c`

  - *mode 2*\
    Running the command\
    `bash src/compile.sh <MAIN_FILE_C> <MODEL_FILE_CPP>`\
    will perform the same tasks as *mode 1*, but using the given model file `<MODEL_FILE_CPP>`
    (extension `.cpp`). This is intended for the user to change the models for the
    TMDPDF and the Collins-Soper kernel. If the model file is not found, the script
    will print a warning message and use the default one.

  - *mode 3*\
    Running the command\
    `bash src/compile.sh <MAIN_FILE_C> <MODEL_FILE_CPP> <EXEC_FILENAME>`\
    will perform the same tasks as mode 2, but naming the executable `<EXEC_FILENAME>`

## Modifying the code

Usually, the user would need to change only (if needed)

- the main program
- the model file

and compile using the `script/compile.sh`
(*mode 2* or *3* if the user made a new model file, see details [here](#functionalities-of-src-compile-sh)).
If some of the other source files need to be modified as well,
the user should produce a new binary for that part of the code. This is simply done
by removing the existing version of the binary before compilation.
For instance, if the user wants to modify the code in `src/pqcd.cpp`,
they should also remove `src/pqcd.o`. Then, running
`script/compile.sh` (with *mode 1*, *2* or *3*) will produce the new version of `src/pqcd.o`



