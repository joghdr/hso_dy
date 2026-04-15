#!/bin/bash
# Category: B1, B2
# Objective: Detect new mutable global state in executable (ratchet)
#            Identify resolved legacy debt in executable (convergence toward zero violations)
#

: "${HSO_ROOT:?[ERROR]: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "[ERROR]: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

export_test_paths

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



