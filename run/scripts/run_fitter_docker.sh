#!/bin/bash
# Run the docker build (see Dockerfile)
catch="${1}"

if [[ -z "${catch_1}" || -z "${catch_2}" ]]; then

  echo  -e \
   "Need 1 argument. Try running:

      bash ${BASH_SOURCE[0]} <OUT_DIR_NAME>

    with argument:

    <OUT_DIR_NAME> - the name of the directory where results should be stored (directory must not exist in 'results/').
    Note: results are stored in 'results/<OUT_DIR_NAME>'"

  exit 1

fi

OUTPUT_DIR=results/"${catch}"

docker run --rm \
-u $(id -u):$(id -g) \
-v $(pwd)/input:/app/input \
-v $(pwd)/results:/app/results \
-t hso-dy:1.0.0 \
HSODrellYanFitter input/kin_drellyan.input input/para_hso.input "${OUTPUT_DIR}"
