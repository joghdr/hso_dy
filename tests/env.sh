#!/bin/bash

set -e

: "${WHY:?}"

: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
: "${HSO_BUILD_DIR:=${HSO_ROOT}/build/dev}"
: "${HSO_BUILD_TYPE:=dev}"


export LC_ALL=C

#test global dirs
export BUILD_DIR="${HSO_BUILD_DIR}"
# export BUILD_DIR="${HSO_ROOT}/${HSO_BUILD_DIR}"
export TEST_DIR="${HSO_ROOT}/tests"
export SOURCE_DIR="${HSO_ROOT}/src"
export INCLUDE_DIR="${HSO_ROOT}/include/hso"


#build-type specific
export AUDIT_DIR="${BUILD_DIR}/audit"

# requires each test.sh script at the same level as
# corresponding baseline/ input/ and output/ directories
# first argument is the full path of the calling script
function export_test_paths {

  local caller_dir="$( cd "$( dirname "${BASH_SOURCE[1]}" )" && pwd )"
  local caller_name="$( basename "${BASH_SOURCE[1]}" )"

  if [[ ! -d "${caller_dir}" ]] ; then
    echo "ERROR: \${caller_dir}='${caller_dir}' not found"
    exit 1
  fi

  if [[ ! -s "${caller_dir}/${caller_name}" ]] ; then
    echo "ERROR: \${caller_dir}/\${caller_name}='${caller_dir}/${caller_name}' not found or empty"
    exit 1
  fi

  export SCRIPT_NAME="${caller_name}"
  export SCRIPT_DIR="${caller_dir}"
  export SCRIPT_PATH="${caller_dir}/${caller_name}"

  #build-independent dirs: input, output, expected
  if [[ -d "${SCRIPT_DIR}/input" ]]; then
    export DATA_INPUT_DIR="${SCRIPT_DIR}/input"
  fi
  if [[ -d "${SCRIPT_DIR}/output" ]]; then
    export DATA_OUTPUT_DIR="${SCRIPT_DIR}/output"
  fi
  if [[ -d "${SCRIPT_DIR}/expected" ]]; then
    export DATA_EXPECTED_DIR="${SCRIPT_DIR}/expected"
  fi
  #build-dependent dirs: baseline
  if [[ -d "${SCRIPT_DIR}/baseline/${HSO_BUILD_TYPE}" ]]; then
    export DATA_BASELINE_DIR="${SCRIPT_DIR}/baseline/${HSO_BUILD_TYPE}"
  fi

}

function export_test_paths_print {

  if [[ -n "${SCRIPT_NAME}"       ]] ; then echo "SCRIPT_NAME       = ${SCRIPT_NAME}      "  ; fi
  if [[ -n "${SCRIPT_DIR}"        ]]  && [[ -d "${SCRIPT_DIR}"        ]] ; then echo "SCRIPT_DIR        = ${SCRIPT_DIR}       "  ; fi
  if [[ -n "${SCRIPT_PATH}"       ]]  && [[ -f "${SCRIPT_PATH}"       ]] ; then echo "SCRIPT_PATH       = ${SCRIPT_PATH}      "  ; fi

  if [[ -n "${DATA_INPUT_DIR}"    ]]  && [[ -d "${DATA_INPUT_DIR}"    ]] ; then echo "DATA_INPUT_DIR    = ${DATA_INPUT_DIR}   "  ; fi
  if [[ -n "${DATA_OUTPUT_DIR}"   ]]  && [[ -d "${DATA_OUTPUT_DIR}"   ]] ; then echo "DATA_OUTPUT_DIR   = ${DATA_OUTPUT_DIR}  "  ; fi
  if [[ -n "${DATA_EXPECTED_DIR}" ]]  && [[ -d "${DATA_EXPECTED_DIR}" ]] ; then echo "DATA_EXPECTED_DIR = ${DATA_EXPECTED_DIR}"  ; fi
  if [[ -n "${DATA_BASELINE_DIR}" ]]  && [[ -d "${DATA_BASELINE_DIR}" ]] ; then echo "DATA_BASELINE_DIR = ${DATA_BASELINE_DIR}"  ; fi

}




