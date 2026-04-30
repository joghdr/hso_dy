#!/bin/bash
# Run the docker build (see Dockerfile)
catch="${1}"

if [[ -z "${catch}" ]]; then

  echo -e "Need to provide output directory name as argument. Try:\n\tOUTPUT_DIR=<dir_name> bash ${BASH_SOURCE[0]}\nResult will be stored in 'run/results/<dir_name>'"

  exit 1

fi

OUTPUT_DIR=results/"${catch}"

docker run --rm \
-u $(id -u):$(id -g) \
-v $(pwd)/input:/app/input \
-v $(pwd)/results:/app/results \
-t hso-dy:1.0.0 \
HSODrellYanFitter input/kin_drellyan.input input/para_hso.input "${OUTPUT_DIR}"
