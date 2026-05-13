#!/bin/bash
# Category: System Validation
# Objective Test consistency with known results

#

set -e


: ${EXEC:? executable was not injected.}

source "${HSO_TEST_HELPER_FILE}"

#TODO: move the discovery of paths into cmake
target_name="${BASH_SOURCE[0]/%.*/}"
target_name="${target_name/#*\//}"
kin_file="${HSO_TEST_SV_INPUT_DIR}/${target_name}/kin_drellyan.input"
para_file="${HSO_TEST_SV_INPUT_DIR}/${target_name}/para_hso.input"
output_dir_name=$( basename $(mktemp -d -t hso_temp_dir_XXXXXXXXXX) )
output_dir="${HSO_TEST_SV_DIR}/${output_dir_name}"

${EXEC} ${kin_file} ${para_file} ${output_dir} > /dev/null

#TODO: move the discovery of expected_file_list into cmake
expected_file_list=( "$(find ${HSO_TEST_SV_EXPECT_DIR}/${target_name} -name "*.dat" )" )

for expected_file in ${expected_file_list}; do

  if [[ -s ${expected_file} ]]; then
    #TODO: move the discovery of output_file into cmake
    output_file="$(find "${output_dir}" -name "$(basename "${expected_file}")")"

    hso_print_msg "Comparing files: " "${expected_file}" "${output_file:-<FILE NOT FOUND>}"

    if [[ !  -f ${output_file} ]]; then

      hso_print_err "Could not find output file to compare to:" "'${expected_file}'"

      exit 1

    fi

    if ! diff ${output_file} ${expected_file}; then

      hso_print_err "Found differences in files:" "output - ${output_file}" "expected - ${expected_file}"

      exit 1

    fi

  fi

done

hso_print_ok "All benchmarks match"


#clean and leave
hso_print_msg "Cheking ${output_dir} "
if [[ -d "${output_dir}" ]]; then
  hso_print_msg "removing ${output_dir} "
  rm -rf "${output_dir}"
fi

hso_print_msg "Cheking /tmp/${output_dir_name} "
if [[ -d /tmp/"${output_dir_name}" ]]; then
  hso_print_msg "removing /tmp/${output_dir_name} "
  rm -rf /tmp/"${output_dir_name}"
fi

exit 0



