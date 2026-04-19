#!/bin/bash
# Category: S3
# Objective: Check header `#ifndef` guards

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
source "${HSO_ROOT}"/tests/env.sh && export_test_paths
source "${HSO_ROOT}"/tests/helpers.sh


function test_header {

  local fname="$1"

  mapfile -t header < <( catclean  "$fname" |  \
                         grep -E '^#(ifndef|define)[[:space:]]+[A-Z0-9_]+' | \
                         head -2 )

  read -r directive_1 name_1 <<< "${header[0]}"

  read -r directive_2 name_2 <<< "${header[1]}"

  if [[ -z "$name_1" ]] || \
     [[ "$directive_1" != '#ifndef'  ]] || \
     [[ "$directive_2" != '#define'  ]] || \
     [[ "$name_1" != "$name_2"  ]] ; then

     print_err \
      "$fname" \
      "header guard not pressent or inconsistent" \
      "${header[0]:-<EMPTY>}" \
      "${header[1]:-<EMPTY>}"


    return 1

  fi

  return 0

}

error_count=0

for fname in "$INCLUDE_DIR"/*.h; do

  test_header "$fname"

  error_count=$(( error_count + $? ))

done

if [[ $error_count > 0 ]]; then

  exit 1

fi

print_ok "no issues found in header guards"

exit 0







