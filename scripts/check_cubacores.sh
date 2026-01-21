#!/bin/bash



kin_file="input/.kin_drellyan_check_cubacores.input"
para_file="input/.para_hso_check_cubacores.input"

src="programs/fit_fixed_target_DY.c"
models="src/models_fcn-tmd-gaussx1-CS-gauss-NEW.cpp"
prog="fit_fixed_target_DY"

max_cores=20

#compile code
bash scripts/compile.sh $src $models $prog

echo -e "time\tCUBACORES\tOMP_NUM_THREADS"
for CUBACORES in $(seq $max_cores); do
    if [ -d results/test ]; then
        rm -rf results/temp_cubacores
    fi
    export CUBACORES=$CUBACORES
    { time  ./exe/$prog "$kin_file" "$para_file" "temp_cubacores" 1> /dev/null  ; } 2> temp_time_cubacores.dat
    grep real temp_time_cubacores.dat | sed -r "s/real//g;s/^[[:space:]]+//g;s/^(.+)$/\1\t$CUBACORES\t$OMP_NUM_THREADS/g"
    rm -rf results/temp_cubacores
    rm temp_time_cubacores.dat
done






