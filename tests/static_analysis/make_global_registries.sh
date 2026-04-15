#!/bin/bash
# Objective: Make registry of  mutable global state in binaries and executable

: "${HSO_ROOT:?ERROR: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "ERROR: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

export_test_paths

if ! { source "${SCRIPT_DIR}/get_global_functions.sh"; }; then
  print_err "Failed to source functions script"
  exit 1
fi

globals_registry_file="${DATA_BASELINE_DIR}/global_registry.sym"

globals_registry_file_app="${DATA_BASELINE_DIR}/global_registry_app.sym"



{ get_globals   > "${globals_registry_file}" \
  && echo "Printed ${globals_registry_file}"; } \
  || echo "Could not print ${globals_registry_file}"


{ get_globals_app > "${globals_registry_file_app}" \
  && echo "Printed ${globals_registry_file_app}";} \
  || echo "Could not print ${globals_registry_file_app}"


exit 0

