#!/bin/bash

set -e

: "${HSO_VERBOSE:=0}"

#message vars
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
function hso_sed_clean {

  : ${1:?"*** [HSO-ERROR] Helper function hso_sed_clean: no filename passed."}

  fname="$1"

  if [[ ! -f "${fname}" ]] ; then

    hso_print_err "Helperfunction hso_sed_clean: file ${fname:-<EMPTY>} not found."

    exit 1
  fi

  sed -E -e 's|//+.*$||g' -e 's|/\*.*\*/||g' -e '/^[[:space:]]*$/d' "${fname}"

}

function hso_print_formatted {

  local tag="$1"

  local caller="${2:-<Unknown Caller>}"

  local tag_color="$3"

  shift 3

  local messages=( "$@" )

  local timestamp="$(date +"%H:%M:%S")"

  printf "%b[%s][%s]%b: %s:\n" "${tag_color}" "HSO-${tag}" "${timestamp}" "${COLOR_NC}" "${caller}" >&2

  for msg in "${messages[@]}"; do

    while IFS= read -r line; do

      printf "   %b%s%b\n" "${COLOR_DIM}" "$line" "${COLOR_NC}" >&2

    done <<< "${msg}"

  done
}

function hso_verbose_gate {

  [[ "${HSO_VERBOSE}" != 0 ]] && return 0 || return 1

}

function hso_get_caller {

  local i

  for i in "${!BASH_SOURCE[@]}"; do

    if [[ "${BASH_SOURCE[$i]}" != "${BASH_SOURCE[0]}" ]]; then

      echo "${BASH_SOURCE[$i]}"

      return 0

    fi

  done

  echo "<Unknown Caller>"

}

function hso_print_err {

  local messages=( "$@" )

  hso_print_formatted "ERROR" "$(hso_get_caller)" "${COLOR_ERR}" "${messages[@]}"

}

function hso_print_warning {

  hso_verbose_gate || return 0

  local messages=( "$@" )

  hso_print_formatted "WARNING" "$(hso_get_caller)" "${COLOR_WARNING}" "${messages[@]}"

}

function hso_print_soft_ok {

  hso_verbose_gate || return 0

  local messages=( "$@" )

  hso_print_formatted "SOFT-OK" "$(hso_get_caller)" "${COLOR_SOFT_OK}" "${messages[@]}"

}

function hso_print_ok {

  hso_verbose_gate || return 0

  local messages=( "$@" )

  hso_print_formatted "OK" "$(hso_get_caller)" "${COLOR_OK}" "${messages[@]}"

}

function hso_print_msg {

  hso_verbose_gate || return 0

  local messages=( "$@" )

  hso_print_formatted "MSG" "" "${COLOR_MSG}" "${messages[@]}"

}




