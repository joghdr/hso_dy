#!/bin/bash
# Category: S2
# Objective: Prevent heavy includes in headers.

set -e

: "${HSO_ROOT_DIR:?" Environment variable was not injected."}"
: "${HSO_INC_DIR:?" Environment variable was not injected."}"
: "${HSO_TEST_HELPER_FILE:?" Helper file was not injected."}"

source "${HSO_TEST_HELPER_FILE}"

violations="$(grep -rEnH  '#include[[:space:]]+<(iostream|fstream|sstream|iomanip|algorithm|random)>'  "${HSO_INC_DIR}" || true)"

if [[ -n "${violations//[[:space:]]/}" ]]; then

  log="$( echo "$violations" | sed 's/#include//g')"

  hso_print_err "heavy includes found:" "${log}"

  exit 1

else

  hso_print_ok "no heavy includes found"

  exit 0

fi


