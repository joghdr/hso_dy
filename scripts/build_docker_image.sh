#!/bin/bash
# Run the docker build pipeline

set -e


: "${HSO_ROOT:=$(git rev-parse --show-toplevel 2> /dev/null || pwd)}"
: "${HSO_BUILD_DIR:=build/dev}"
: "${HSO_BUILD_TYPE:=dev}"
: "${IMAGE_NAME:=hso}"
: "${VERSION:=-}"

# echo "------------------------------------------------------------------  ${IMAGE_NAME}:v${VERSION} "; exit 1


source ${HSO_ROOT}/tests/env.sh && export_test_paths
source "${HSO_ROOT}/tests/helpers.sh"
# source "${SCRIPT_DIR}/get_global_functions.sh"

docker build -t ${IMAGE_NAME}:v${VERSION} .



