#!/bin/bash
# Category: B1, B2
# Objective: Detect new mutable global state in binaries(ratchet)
#            Identify resolved legacy debt in binaries(convergence toward zero violations)
#

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
: "${HSO_BUILD_DIR:=build/dev}"
: "${HSO_BUILD_TYPE:=dev}"
source "${HSO_ROOT}"/tests/env.sh && export_test_paths
source "${HSO_ROOT}"/tests/helpers.sh


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



