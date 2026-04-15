#!/bin/bash
# Category: S5
# Objective:

: "${HSO_ROOT:?[ERROR]: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
: "${HSO_BUILD_DIR:?[ERROR]: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "[ERROR]: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

export_test_paths

report_path="$AUDIT_DIR/iwyu_report.txt"

all_includes=( "-I" "${INCLUDE_DIR}" )

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


iwyu_tool -j $cores -p "${BUILD_DIR}" -- "${all_includes[@]}" > "$report_path" 2>&1

fatal="$( grep -E "fatal error:" "$report_path" | wc -l)"

errors="$( grep -E "error:" "$report_path" | grep -v "fatal"  | wc -l)"

violations="$( grep -E "should (add|remove)|forward declarations" "$report_path" | wc -l)"


if (( fatal > 0 )); then

  print_err \
  "Found ${fatal} errors:" \
  "$(grep -w "fatal" "$report_path" | sort | uniq -c | sort -nr)" \
  "Fatal errors should be fixed before addressing violations"

  exit 1

fi


if (( errors > 0 )); then

  print_err \
  "Found ${errors} errors:" \
  "$(grep -w "error" "$report_path" | grep -v "fatal" | sort | uniq -c | sort -nr)" \
  "Errors should be fixed before addressing violations"



  exit 1

fi


if [[ "$violations" > 0 ]]; then

  print_err "violations found:"

  print_msg \
  "See ${report_path} for details" \
  "$(grep -E  "should (add|remove)|forward declarations" \
  "$report_path" | awk '{print $1}' | sort | uniq -c | sort -nr)"


  exit 1

fi

print_ok "no violations found"

exit 0













