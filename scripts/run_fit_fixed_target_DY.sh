#!/bin/bash


kin_file="input/kin_drellyan.input"
para_file="input/para_hso.input"
output_dir="$1"

src="programs/fit_fixed_target_DY.c"
models="src/models_fcn-tmd-gaussx1-CS-gauss.cpp"
prog="fit_fixed_target_DY"

#compile code
bash scripts/compile.sh $src $models $prog

# env variables
# use scripts/check_cubacores.sh to find fastest
export CUBACORES=5
# use always 1 for OMP_NUM_THREADS
export OMP_NUM_THREADS=1

echo "running with CUBACORES = $CUBACORES"

echo "running with OMP_NUM_THREADS = $OMP_NUM_THREADS"

#run
echo "#running with CUBACORES=$CUBACORES"

time ./exe/$prog "$kin_file" "$para_file" $output_dir










