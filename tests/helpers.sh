#!/bin/bash

#message vars
# Define colors using tput
if [ -t 2 ]; then
  COLOR_OK=$(tput setaf 10)
  COLOR_DIM=$(tput setaf 7)
  COLOR_SOFT_OK=$(tput setaf 22)
  COLOR_WARNING=$(tput setaf 11)
  COLOR_ERR=$(tput setaf 196)
  COLOR_MSG=$(tput setaf 12)
  COLOR_NC=$(tput sgr0)
else

  COLOR_OK=''
  COLOR_DIM=''
  COLOR_SOFT_OK=''
  COLOR_WARNING=''
  COLOR_ERR=''
  COLOR_MSG=''
  COLOR_NC=''


fi


##general helpers
function catclean {

  fname="$1"

  cat "$fname" | \
  sed -E 's|//+.*$||g' | \
  sed -E 's|/\*.*\*/||g' | \
  sed -E '/^[[:space:]]*$/d'

}

function print_formatted {

  local tag="$1"

  local caller="$2"

  local tag_color="$3"

  shift 3

  local messages=( "$@" )

  printf "%b[%s]%b: %s:\n" "${tag_color}" "${tag}" "${COLOR_NC}" "${caller}" >&2

  for msg in "${messages[@]}"; do

    while IFS= read -r line; do

      printf "   %b%s%b\n" "${COLOR_DIM}" "$line" "${COLOR_NC}" >&2

    done <<< "${msg}"

  done
}

function print_err {

  local messages=( "$@" )

  print_formatted "ERROR" "${BASH_SOURCE[1]}" "${COLOR_ERR}" "${messages[@]}"

}

function print_warning {

  local messages=( "$@" )

  print_formatted "WARNING" "${BASH_SOURCE[1]}" "${COLOR_WARNING}" "${messages[@]}"

}

function print_soft_ok {

  local messages=( "$@" )

  print_formatted "SOFT-OK" "${BASH_SOURCE[$(( ${#BASH_SOURCE[@]} - 1 ))]}" "${COLOR_SOFT_OK}" "${messages[@]}"

}

function print_ok {

  local messages=( "$@" )

  print_formatted "OK" "${BASH_SOURCE[$(( ${#BASH_SOURCE[@]} - 1 ))]}" "${COLOR_OK}" "${messages[@]}"

}



function print_msg {

  local messages=( "$@" )

  print_formatted "MSG" "" "${COLOR_MSG}" "${messages[@]}"

}




