#!/bin/bash
# Category: S1
# Objective: Forbid static symbols in Headers.

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"

source "${HSO_ROOT}"/tests/env.sh && export_test_paths
source "${HSO_ROOT}"/tests/helpers.sh

VIOLATIONS=$(catclean <(grep -rwE "static"  "$INCLUDE_DIR" ) | \
              grep -vE "static_cast|static_assert" || true  )

if [[ -n "${VIOLATIONS//[[:space:]]/}" ]]; then

  print_err "Static symbols found in header" "$VIOLATIONS"

  exit 1

else

  print_ok "Header is free of static symbols"

  exit 0

fi


