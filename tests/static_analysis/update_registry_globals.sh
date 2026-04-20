#!/bin/bash
# Objective: Update registry of  mutable global state in binaries and executable


: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
: "${HSO_BUILD_DIR:=${HSO_ROOT}/build/dev}"
: "${HSO_BUILD_TYPE:=dev}"

source ${HSO_ROOT}/tests/env.sh && export_test_paths
source "${HSO_ROOT}/tests/helpers.sh"
source "${SCRIPT_DIR}/get_global_functions.sh"

globals_registry_file="${DATA_BASELINE_DIR}/global_registry.sym"

globals_registry_file_app="${DATA_BASELINE_DIR}/global_registry_app.sym"



get_globals > "${globals_registry_file}" && \
echo "Printed ${globals_registry_file}" || \
echo "Could not print ${globals_registry_file}"

get_globals_app > "${globals_registry_file_app}" && \
echo "Printed ${globals_registry_file_app}" || \
echo "Could not print ${globals_registry_file_app}"


exit 0

