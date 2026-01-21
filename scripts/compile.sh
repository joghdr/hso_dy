#!/bin/bash



flags="-Wall -Werror -Wfatal-errors -O2 -std=c++20"

include="-I $(pwd)/include/"

libraries="-lLHAPDF -lMinuit2 -lgomp -lgsl -lgslcblas -lcuba  -lstdc++fs"



### check input arguments
if [ "$#" -lt 1 ] ; then
    echo -e "\n---------> need at least one argument:\n\t\t bash src/compile.sh <MAIN_FILE_C>"
    echo -e "---------> alternatively:\n\t\t bash src/compile.sh <MAIN_FILE_C> <MODEL_FILE_CPP>"
    echo -e "---------> or:\n\t\t bash src/compile.sh <MAIN_FILE_C> <MODEL_FILE_CPP> <EXEC_FILENAME>"
    exit 0
fi

declare input="./$1"
declare models_fcn="$2"
declare output="./exe/$3"
declare objects_data="src/objects_data.cpp"
declare objects_theory="src/objects_theory.cpp"
declare objects_stat="src/objects_stat.cpp"



if [ "$models_fcn" == "" ] | [ ! -f "$models_fcn"  ] ; then

    declare models_fcn_passed="$models_fcn"
    
    models_fcn="src/models_fcn-tmd-gaussx1-CS-gauss.cpp"
    
    echo -e "\n---------> Warning: could not find model file '$models_fcn_passed'"
    
    echo -e "                    using default model '$models_fcn' \n"

fi

if [ "$output" == "./exe/" ] ; then

    output_fname=$(echo $1 | sed -r "s/(.+)\..+/\1/g" | sed -r "s/.*\/(.+)/\1/g")

    output="./exe/$output_fname"

    echo -e "---------> using default exec name: $output\n"


fi


declare -a cppfiles=( \
src/FCN.cpp \
src/utilities.cpp \
src/collinear_factorization.cpp \
src/csec_DY_css_hso_integrands_cuba.cpp \
src/WDY_hso.cpp \
src/tmdPDF.cpp \
src/evo.cpp \
src/CSkernel.cpp \
src/scale_setting.cpp \
src/pdf_cutoff_from_msbar.cpp \
src/pqcd.cpp \
src/math_functions.cpp \
src/stat_class.cpp \
src/theory_class.cpp \
src/data_class.cpp \
src/read_data.cpp \
src/read_para.cpp \
src/read_kin.cpp \
src/behavior.cpp \
)




for cppfile in ${cppfiles[@]}  ;do

  declare binary="$(echo $cppfile|sed "s/\.cpp/.o/g")"

  if [ ! -f $binary ] ; then

    echo -e -n "---------> compiling $cppfile  ...  "

    g++  $flags -c $cppfile  $include  $libraries -o  $binary

    echo -e "done: $binary"

  fi

done



declare  binaries="$(for cppfile in ${cppfiles[@]};do echo $cppfile|sed "s/\.cpp/.o/g";done)"

declare -a cppdts=( \
$objects_data \
$objects_theory \
$objects_stat\
)

echo -e -n "\n---------> compiling ${cppdts[@]}  ...  "

g++  $flags -c ${cppdts[@]}  $include $libraries

mv objects_data.o src/
mv objects_theory.o src/
mv objects_stat.o src/
                                 
echo -e "done\n"



binaries="$binaries src/objects_data.o src/objects_theory.o src/objects_stat.o"

g++  $flags $input $models_fcn $binaries  $include $libraries -o $output

echo -e "\n---------> executable:\t$(ls $output)\n"
    

