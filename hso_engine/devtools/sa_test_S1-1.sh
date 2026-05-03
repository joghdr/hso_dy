#!/bin/bash
# Category: S1
# Objective: Forbid static symbols in Headers.

set -e

: "${HSO_ROOT_DIR:?" Environment variable was not injected."}"
: "${HSO_INC_DIR:?" Environment variable was not injected."}"
: "${HSO_TEST_HELPER_FILE:?" Helper file was not injected."}"


source "${HSO_TEST_HELPER_FILE}"

violations=$(hso_sed_clean <(grep -rwE "static"  "${HSO_INC_DIR}" ) | \
              grep -vE "static_cast|static_assert" || true  )



if [[ -n "${violations//[[:space:]]/}" ]]; then

  hso_print_err "Static symbols found in header" "${violations}"

  exit 1

else

  hso_print_ok  "Header is free of static symbols"

  exit 0

fi


