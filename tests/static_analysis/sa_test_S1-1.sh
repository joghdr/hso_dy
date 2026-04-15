#!/bin/bash
# Category: S1
# Objective: Forbid static symbols in Headers.

: "${HSO_ROOT:?ERROR: variable not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "ERROR: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

export_test_paths

VIOLATIONS=$(catclean <(grep -rwE "static"  "$INCLUDE_DIR" ) | \
              grep -vE "static_cast|static_assert"  )

if [[ -n "${VIOLATIONS//[[:space:]]/}" ]]; then

  print_err "Static symbols found in header" "$VIOLATIONS"

  exit 1

else

  print_ok "Header is free of static symbols"

  exit 0

fi


