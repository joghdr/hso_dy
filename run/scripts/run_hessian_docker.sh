#!/bin/bash
# Run the docker build (see Dockerfile)
catch_1="${1}"
catch_2="${2}"

if [[ -z "${catch_1}" || -z "${catch_2}" ]]; then

  echo  -e \
   "Need 2 arguments. Try running:

      bash ${BASH_SOURCE[0]} <FIT_DIR_NAME> <OUT_DIR_NAME>

    with arguments:

    <FIT_DIR_NAME> - the name of a directory containing results of a fit (run the script 'run_fitter_docker.sh')
    <OUT_DIR_NAME> - the name of the directory where results should be stored (directory must not exist)"

  exit 1

fi


FIT_DIR="${catch_1}"
OUTPUT_DIR=results/"${catch_2}"

docker run --rm \
-u $(id -u):$(id -g) \
-v $(pwd)/results:/app/results \
-it hso-dy:1.0.0 \
HSODrellYanHessian "${FIT_DIR}" "${OUTPUT_DIR}"
