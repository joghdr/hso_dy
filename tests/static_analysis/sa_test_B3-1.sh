#!/bin/bash
# Category: B1, B2
# Objective: Detect new mutable global state in executable (ratchet)
#            Identify resolved legacy debt in executable (convergence toward zero violations)
#

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
source "${HSO_ROOT}"/tests/env.sh && export_test_paths
source "${HSO_ROOT}"/tests/helpers.sh


if ! { source "${SCRIPT_DIR}/get_global_functions.sh"; }; then
  echo "[ERROR]: ${SCRIPT_NAME} failed to source function script" >&2
  exit 1
fi

globals_registry_file_app="${DATA_BASELINE_DIR}/global_registry_app.sym"

if ! find_new_globals_app "${globals_registry_file_app}"; then
  exit 1
fi


if ! find_resolved_globals_app "${globals_registry_file_app}"; then
  exit 1
fi

exit 0



