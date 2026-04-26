#!/bin/bash
# Run the docker build (see Dockerfile)
OUTPUT_DIR="${1}"

if [[ -z "${OUTPUT_DIR}" ]]; then

  echo -e "Need to provide output directory name as argument. Try:\n\tOUTPUT_DIR=<dir_name> bash ${BASH_SOURCE[0]}\nResult will be stored in 'run/results/<dir_name>'"

  exit 1

fi

docker run --rm \
-u $(id -u):$(id -g) \
-v $(pwd)/input:/app/run/input \
-v $(pwd)/results:/app/run/results \
-it hso-dy:1.0.0 \
HSODrellYanFitter run/input/kin_drellyan.input run/input/para_hso.input "${OUTPUT_DIR}"
