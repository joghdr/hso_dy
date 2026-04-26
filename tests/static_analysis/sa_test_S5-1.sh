#!/bin/bash
# Category: S5
# Objective:

set -e

: "${HSO_ROOT_DIR:?" Environment variable was not injected."}"
: "${HSO_INC_DIR:?" Environment variable was not injected."}"
: "${HSO_DEV_TOOLS_DIR:?" Environment variable was not injected."}"
: "${HSO_BUILD_DIR:?" Environment variable was not injected."}"
: "${HSO_AUDIT_DIR:?" Environment variable was not injected."}"
: "${HSO_TEST_HELPER_FILE:?" Helper file was not injected."}"

source "${HSO_TEST_HELPER_FILE}"


report_file="${HSO_AUDIT_DIR}/iwyu_report.txt"

if [[ -f ${report_file} ]]; then

  hso_print_msg "${report_file} exists. Deleting."

  rm "${report_file}"

fi

all_includes=( "-I" "${HSO_INC_DIR}" )

system_includes="$(echo | gcc -E -v -xc++ - 2>&1 | \
                  grep -A 100 "#include <...> search starts here:" | \
                  grep -B 100 "End of search list." | \
                  grep "^ /" | sed 's/^ //')"

for dir in $system_includes; do

  if [[ "$dir" == *"/gcc/x86_64-linux-gnu/13/include"* ]]; then

    continue

  fi

  if [ -d "$dir" ]; then

      all_includes+=("-isystem" "$dir")

  fi

done

clang_res_dir=$(clang -print-resource-dir)

if [[ -d "$clang_res_dir/include" ]]; then

  all_includes+=("-isystem" "$clang_res_dir/include")

fi

if [[ "$ostype" == "darwin"* ]]; then

  cores=$(sysctl -n hw.logicalcpu)

else

  cores=$(nproc)

fi


iwyu_mapping="${HSO_DEV_TOOLS_DIR}/hso.imp"

if [[ ! -f "${iwyu_mapping}" ]] ; then

  hso_print_err "File ${iwyu_mapping:-<EMPTY>} not found."

  return 1

fi

echo "${HSO_BUILD_DIR}"

iwyu_tool.py -j $cores -p "${HSO_BUILD_DIR}" -- -Xiwyu --mapping_file="${iwyu_mapping}" > "$report_file" 2>&1

# iwyu_tool.py -j $cores -p "${HSO_BUILD_DIR}" -- \
# -Xiwyu --mapping_file="${iwyu_mapping}" \
# "${all_includes[@]}" > "$report_file" 2>&1

fatal="$( grep -E "fatal error:" "$report_file" | wc -l)"

errors="$( grep -E "error:" "$report_file" | grep -v "fatal"  | wc -l)"

violations="$( grep -E "should (add|remove)|forward declarations" "$report_file" | wc -l)"


if [[ ! -s ${report_file} ]]; then
  hso_print_err "file report_file=${report_file:-<EMPTY>} not found" \
                "Check that it is being produced."
  exit 1
fi

if (( fatal > 0 )); then

  hso_print_err \
  "Found ${fatal} errors:" \
  "$(grep -w "fatal" "$report_file" | sort | uniq -c | sort -nr)" \
  "Fatal errors should be fixed before addressing violations"

  exit 1

fi

if (( errors > 0 )); then

  hso_print_err \
  "Found ${errors} errors:" \
  "$(grep -w "error" "$report_file" | grep -v "fatal" | sort | uniq -c | sort -nr)" \
  "Errors should be fixed before addressing violations"

  exit 1

fi

if [[ "$violations" > 0 ]]; then


  hso_print_err "violations found:" \
  "See ${report_file} for details" \
  "$(grep -E  "should (add|remove)|forward declarations" \
  "$report_file" | awk '{print $1}' | sort | uniq -c | sort -nr)"

  exit 1

fi

hso_print_ok "no violations found"

exit 0



