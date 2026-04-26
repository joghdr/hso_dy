#!/bin/bash
# Category: B1, B2
# Objective: Detect new mutable global state in binaries(ratchet)
#            Identify resolved legacy debt in binaries(convergence toward zero violations)
#

set -e

: "${HSO_TEST_HELPER_FILE:?" Helper file was not injected."}"
: "${HSO_TEST_SA_HELPER_FILE:?" Helper file was not injected."}"
: "${HSO_REG_GLOBALS_FILE:?" Helper file was not injected."}"


source "${HSO_TEST_SA_HELPER_FILE}"


if ! find_new_globals "${HSO_REG_GLOBALS_FILE}"; then
  exit 1
fi


if ! find_resolved_globals "${HSO_REG_GLOBALS_FILE}"; then
  exit 1
fi

exit 0



