#!/bin/bash
# Category: S2
# Objective: Prevent heavy includes in headers.

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
source "${HSO_ROOT}"/tests/env.sh && export_test_paths
source "${HSO_ROOT}"/tests/helpers.sh


VIOLATIONS="$(grep -rEnH  '#include[[:space:]]+<(iostream|fstream|sstream|iomanip|algorithm|random)>'  "$INCLUDE_DIR" || true)"

if [[ -n "${VIOLATIONS//[[:space:]]/}" ]]; then

  LOG="$( echo "$VIOLATIONS" | sed 's/#include//g')"

  print_err "heavy includes found:" "${LOG}"

  exit 1

else

  print_ok "no heavy includes found"

  exit 0

fi


