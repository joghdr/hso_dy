#!/bin/bash
# Category: S2
# Objective: Prevent heavy includes in headers.

: "${HSO_ROOT:?[ERROR]: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "[ERROR]: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

export_test_paths

VIOLATIONS="$(grep -rEnH  '#include[[:space:]]+<(iostream|fstream|sstream|iomanip|algorithm|random)>'  "$INCLUDE_DIR" )"


if [[ -n "${VIOLATIONS//[[:space:]]/}" ]]; then

  LOG="$( echo "$VIOLATIONS" | sed 's/#include//g')"

  print_err "heavy includes found:" "${LOG}"

  exit 1

else

  print_ok "no heavy includes found"

  exit 0

fi


