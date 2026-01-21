#!/bin/bash

run_script="scripts/run_fit_fixed_target_DY.sh"

check_dir="temp_test"

control_dir="control/E288_control_fast"

control_files=( $control_dir/min/*  $control_dir/input/*  $control_dir/status/*  $control_dir/stat/E288/* )


if [ -d "$check_dir" ]; then
    rm -rf $check_dir
fi

echo "#    copying input files from  '$control_dir'"

mkdir temp_bk

cp input/kin_drellyan.input  temp_bk

cp input/para_hso.input temp_bk

cp $control_dir/input/kin_drellyan.input  input/

cp $control_dir/input/para_hso.input  input/

echo "#    running '$run_script'"

bash $run_script $check_dir &> /dev/null

mv results/$check_dir .

echo "#    comparing '$check_dir' to '$control_dir'"
echo "#    if checks succeed next line prints 'END OF CHECK'"

for benchmark in ${control_files[@]}; do
    fname=$benchmark
    fname=$(echo ${fname#*/})
    fname=$(echo ${fname#*/})

    file_to_test=$check_dir/$fname

    diff $file_to_test $benchmark

done

echo "#    END OF CHECK"

cp temp_bk/kin_drellyan.input  input

cp temp_bk/para_hso.input input

rm -rf temp_bk

rm -rf $check_dir

echo "--------------------$check_dir"




