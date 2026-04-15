#!/bin/bash
# Category: B1, B2
# Objective: Detect new mutable global state in binaries(ratchet)
#            Identify resolved legacy debt in binaries(convergence toward zero violations)
#

: "${HSO_ROOT:?[ERROR]: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "[ERROR]: ${SCRIPT_NAME}: failed to source scripts" >&2
  exit 1
fi

export_test_paths

if ! { source "${SCRIPT_DIR}/get_global_functions.sh"; }; then
  echo "[ERROR]: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

globals_registry_file="${DATA_BASELINE_DIR}/global_registry.sym"


if ! find_new_globals "${globals_registry_file}"; then
  exit 1
fi


if ! find_resolved_globals "${globals_registry_file}"; then
  exit 1
fi

exit 0



