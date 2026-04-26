#!/bin/bash
# Objective: Update registry of  mutable global state in binaries and executable

set -e

: "${HSO_TEST_SA_HELPER_FILE:?" Helper file was not injected."}"
: "${HSO_REG_GLOBALS_FILE:?" Helper file was not injected."}"
: "${HSO_REG_GLOBALS_APP_FILE:?" Helper file was not injected."}"

source "${HSO_TEST_SA_HELPER_FILE}"

get_globals > "${HSO_REG_GLOBALS_FILE}" && \
echo "Printed ${HSO_REG_GLOBALS_FILE}" || \
echo "Could not print ${HSO_REG_GLOBALS_FILE}"


get_globals_app > "${HSO_REG_GLOBALS_APP_FILE}" && \
echo "Printed ${HSO_REG_GLOBALS_APP_FILE}" || \
echo "Could not print ${HSO_REG_GLOBALS_APP_FILE}"


exit 0

