#!/bin/bash
# Category: S3
# Objective: Check header `#ifndef` guards

set -e

: "${HSO_ROOT_DIR:?" Environment variable was not injected."}"
: "${HSO_INC_DIR:?" Environment variable was not injected."}"
: "${HSO_TEST_HELPER_FILE:?" Helper file was not injected."}"

source "${HSO_TEST_HELPER_FILE}"

function hso_test_header {

  local fname="${1}"

  if [[ ! -f "${fname}" ]] ; then

     hso_print_err "Helper function hso_test_header file ${fname:-<EMPTY>} not found."

     exit 1

  fi


  mapfile -t header < <( hso_sed_clean  "$fname" |  \
                         grep -E '^#(ifndef|define)[[:space:]]+[A-Z0-9_]+' | \
                         head -2 )

  read -r directive_1 name_1 <<< "${header[0]}"

  read -r directive_2 name_2 <<< "${header[1]}"

  if [[ -z "$name_1" ]] || \
     [[ "$directive_1" != '#ifndef'  ]] || \
     [[ "$directive_2" != '#define'  ]] || \
     [[ "$name_1" != "$name_2"  ]] ; then

     hso_print_err \
      "$fname" \
      "header guard not pressent or inconsistent" \
      "${header[0]:-<EMPTY>}" \
      "${header[1]:-<EMPTY>}"


    return 1

  fi

  return 0

}

error_count=0

for fname in $(find "${HSO_INC_DIR}" -name "*.h" ); do

  hso_test_header "$fname"

  error_count=$(( error_count + $? ))

done

if [[ $error_count > 0 ]]; then

  exit 1

fi

hso_print_ok "no issues found in header guards"

exit 0







