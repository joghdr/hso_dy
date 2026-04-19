#!/bin/bash
# Category: System Validation
# Objective Test consistency with known results

set -e

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
: "${HSO_BUILD_DIR:=build/dev}"
: "${EXEC:=${HSO_ROOT}/build/dev/bin/HSODrellYanFitter}"

source ${HSO_ROOT}/tests/env.sh && export_test_paths
source "${HSO_ROOT}/tests/helpers.sh"


SCRIPT_NAME="$( basename "${BASH_SOURCE[0]}" )"

SCRIPT_DIR="$( cd $(dirname "${BASH_SOURCE[0]}") && pwd  )"

kin_file="${SCRIPT_DIR}/input/kin_drellyan.input"

para_file="${SCRIPT_DIR}/input/para_hso.input"

output_root_dir="temp"

output_dir="${output_root_dir}/stat/E288"

expected_dir="${SCRIPT_DIR}/expected/E288_control_fast/stat/E288"

if [[ -d results/${output_root_dir} ]]; then
  rm -rf "results/${output_root_dir}"
fi


${EXEC} ${kin_file} ${para_file} ${output_root_dir} > /dev/null

for expected_file in "${expected_dir}"/*.stat; do

  if [[ -s ${expected_file} ]]; then

    output_file="results/${output_dir}/$(basename ${expected_file})"

    if [[ !  -f ${output_file} ]]; then

      print_err "Could not find output file '${output_file}'"

      exit 1

    fi

    if ! diff ${output_file} ${expected_file}; then

      print_err "Found differences in files:" "output - ${output_file}" "expected - ${expected_file}"

      exit 1

    fi

  fi

done

print_ok "All benchmarks match"

exit 0



