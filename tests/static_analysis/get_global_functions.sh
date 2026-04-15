
: "${HSO_ROOT:?ERROR: ${SCRIPT_NAME} HSO_ROOT not set (check the MakeFile)}"
if ! { source "${HSO_ROOT}/tests/env.sh" && source "${HSO_ROOT}/tests/helpers.sh"; }; then
  echo "ERROR: ${SCRIPT_NAME} failed to source scripts" >&2
  exit 1
fi

SCRIPT_NAME="$( basename "${BASH_SOURCE[0]}" )"


#explicit global symbols (binaries)
function get_globals_raw {

  if [[ !  -s "$OBJ_PATH_FILE" ]]; then

    print_err "$OBJ_PATH_FILE is empty or it does not exist"

    return 1

  fi

  local binaries_path

  binaries_path="$(cat "$OBJ_PATH_FILE" 2> /dev/null )"

  if [[ !  -d "$binaries_path" ]]; then

    print_err "could not find $binaries_path"

    return 1

  fi

  ##gate to prevent comparing to a broken build
  local expected_count=$(find "$SOURCE_DIR" -name "*.cpp" | wc -l)

  local count=$(find "$binaries_path" -name "*.o" | wc -l)

  if [[ "$count" -lt "$expected_count" ]]; then

    print_err "Build is incomplete. Expected $expected_count objects, found $count."

    return 1

  fi


  local global_symbol

  global_symbol="$( \
        find "$binaries_path" -name "*.o" -print0 | \
        xargs -0 -r nm -AC  | \
        grep ' [DBGSdbgs] ' | \
        sed -E "s/(.+:).+ (.) /\1 \2 /g" | \
        sed -E "s/\[.+\]//g" | \
        sort 2> /dev/null \
        )"

  if [[ -n "${global_symbol//[[:space:]]/}" ]]; then

    printf "$global_symbol" | tr -d '\r' | sort

  fi

  return 0


}

function get_globals {

  if [[ !  -d "$BUILD_DIR" ]]; then

    print_err "Build dir '${BUILD_DIR}' not found. Run cmake chain first."

    return 1

  fi

  get_globals_raw

}

function diff_with_registry {

  local globals_registry_file="$1"

  diff <(get_globals | awk '{ print $(NF-1), $NF }' | sort) <(cat "$globals_registry_file" | awk '{ print $(NF-1), $NF }' | sort)

}

function find_new_globals {

  local globals_registry_file="$1"

  if [[ ! -f "${globals_registry_file}" ]]; then

    print_err "Registry file ${globals_registry_file} not present (binaries)" "See MakeFile to generate a new registry"

    exit 1

  fi

  local current_globals

  current_globals=$(get_globals 2>&1)

  local exit_code=$?

  if [[ $exit_code -ne 0 ]]; then

    print_err "Function 'get_globals' unexpectedly failed."

    return 1

  fi

  local current_globals_clean="$(echo "$current_globals"  | sed 's/\[abi:[^]]*\]//g' | awk '{ print $NF }' | tr -d '\r' | sort -u )"

  local registry_globals_clean="$(cat "$globals_registry_file" | sed 's/\[abi:[^]]*\]//g' | awk '{ print $NF }' | tr -d '\r' | sort -u )"

  local new_globals="$(comm -23 <(echo "$current_globals_clean") <(echo "$registry_globals_clean") )"

  if [[ -n "${new_globals//[[:space:]]/}" ]]; then

    print_err "Found new globals in binaries:" "$new_globals"

    print_msg " REGISTRY LIST IS:" "${globals_registry_file}"

    echo "${registry_globals_clean}" >&2

    print_msg " CURRENT GLOBALS:"

    echo "${current_globals_clean}" >&2

    return 1

  fi

  print_soft_ok "No new globals found in binaries"

  return 0

}

function find_resolved_globals {

  local globals_registry_file="$1"

  if [[ ! -f "${globals_registry_file}" ]]; then

    print_err "Registry file ${globals_registry_file} not present (binaries)" "See MakeFile to generate a new registry"

    exit 1

  fi

  local current_globals

  current_globals=$(get_globals 2>&1)

  local exit_code=$?

  if [[ $exit_code -ne 0 ]]; then

    echo "$current_globals" >&2

    return 1

  fi

  local resolved_globals="$(comm -13 <(echo "$current_globals" | awk '{ print $NF }' | sort -u ) <(cat "$globals_registry_file" | awk '{ print $NF }' | sort -u ) )"

  if [[ -n "${resolved_globals//[[:space:]]/}" ]]; then

    if [ ! -f /.dockerenv ]; then
      print_soft_ok "Some globals tagged as 'resolved' while auditing in container. Ignoring (harmless)."
      return 0
    else

      print_warning \
      "Found new resolved globals in binaries, registry must be updated," \
      "see Makefile for details." \
      "Resolved globals:" \
      "${resolved_globals}"

      return 1

    fi

  fi

  print_soft_ok "All global variable instances reported in registry (binaries)."

  return 0

}


#implicit global symbols (app level)
function get_globals_raw_app {

  if [[ !  -s "$BIN_PATH_FILE" ]]; then

    print_err "BIN_PATH_FILE='${BIN_PATH_FILE}' is empty or it does not exist"

    return 1

  fi

  local app_path="$(cat "$BIN_PATH_FILE" 2> /dev/null )"

  if [[ !  -f "$app_path" ]]; then

    print_err "Could not find path to app=${app_path}"

    return 1

  fi

  local global_symbol_app

  global_symbol_app="$( \
    nm -C  "$app_path" | \
    grep ' [dbgs] ' | \
    grep -vE 'vtable|typeinfo|VTT' | \
    sed -E "s/.+ (.) /\1 /g" | \
    sed  -E "s/\[.+\]//g" | \
    sort 2> /dev/null
  )"

  if [[ -n "${global_symbol_app//[[:space:]]/}" ]]; then

    echo "$global_symbol_app"

  fi


  return 0

}

function get_globals_app {

  if [[ !  -d "$BUILD_DIR" ]]; then

    print_err "Build directory '$BUILD_DIR' not found. Run cmake chain first."

    return 1

  fi

  local global_symbol_names="$( get_globals | sed -E "s/^.+ . //g" | sort)"

  local global_symbol_names_app="$( get_globals_raw_app | sed -E "s/^. //g; s/\[.+\]//g" | sort)"

  local global_symbol_names_disjoint="$(comm -23 <(echo "$global_symbol_names_app") <(echo "$global_symbol_names") | sed -E "s/.+://g")"

  local global_symbol_names_new=""

  for candidate in $(echo "$global_symbol_names_disjoint"); do

    if grep -rwqs "$candidate" "include" "src"; then

      global_symbol_names_new="$global_symbol_names_new\n""$candidate"

    fi

  done

  global_symbol_names_new="$(echo -e "$global_symbol_names_new" | sed "/^$/d" | sort -u)"

  global_symbol_names_app="$( get_globals_raw_app | sort)"

  local app_path="$(cat "$BIN_PATH_FILE"  )"

  local global_symbol_new_full_entry=(  )

  for new_symbol in $(echo "$global_symbol_names_new"); do

    readarray -t -O "${#global_symbol_new_full_entry[@]}" global_symbol_new_full_entry < <(echo "${global_symbol_names_app}" | grep -w ${new_symbol})

  done


  if [[ -n "${global_symbol_new_full_entry[0]//[[:space:]]/}" ]]; then

    printf "%s\n" "${global_symbol_new_full_entry[@]}" | sed "/^$/d" | sort

  fi

  return 0

}

function diff_with_registry_app {

  local globals_registry_file_app="$1"

  diff <(get_globals_app | awk '{ print $(NF-1), $NF }' | sort) <(cat "$globals_registry_file_app" | awk '{ print $(NF-1), $NF }' | sort)

}

function find_new_globals_app {

  local globals_registry_file_app="$1"

  if [[ ! -f "${globals_registry_file_app}" ]]; then

    print_err "Registry file '${globals_registry_file_app}' not present (executable)" "See MakeFile to generate a new registry"

    exit 1

  fi

  local current_globals="$(get_globals_app 2>&1)"

  local current_globals_clean="$(echo "$current_globals"  | sed 's/\[abi:[^]]*\]//g' | awk '{ print $NF }' | tr -d '\r' | sort -u )"

  local registry_globals_clean="$(cat "$globals_registry_file_app" | sed 's/\[abi:[^]]*\]//g' | awk '{ print $NF }' | tr -d '\r' | sort -u )"

  local new_globals="$(comm -23 <(echo "$current_globals_clean") <(echo "$registry_globals_clean") )"


  if [[ -n "${new_globals//[[:space:]]/}" ]]; then

    print_err "Found new globals in executable" "$new_globals"

    return 1

    fi

    print_soft_ok "No new globals found in executable."

    return 0

}

function find_resolved_globals_app {

  local globals_registry_file="$1"

  if [[ ! -f "${globals_registry_file_app}" ]]; then

    print_err "Registry file '${globals_registry_file_app}' not present (executable)" "See MakeFile to generate a new registry"

    exit 1

  fi

  local current_globals="$(get_globals_app 2>&1)"

  local current_globals_clean="$(echo "$current_globals"  | sed 's/\[abi:[^]]*\]//g' | awk '{ print $NF }' | tr -d '\r' | sort -u )"

  local registry_globals_clean="$(cat "$globals_registry_file_app" | sed 's/\[abi:[^]]*\]//g' | awk '{ print $NF }' | tr -d '\r' | sort -u )"

  local resolved_globals="$(comm -13 <(echo "$current_globals_clean") <(echo "$registry_globals_clean") )"


  if [[ -n "${resolved_globals//[[:space:]]/}" ]]; then

    if [ ! -f /.dockerenv ]; then
      print_soft_ok "Some globals tagged as 'resolved' while auditing in container. Ignoring (harmless)."
      return 0
    else

      print_warning \
      "Found new resolved globals in executable, registry must be updated," \
      "see Makefile for details." \
      "Resolved globals:" \
      "${resolved_globals}"

      print_msg "CURRENT" "${current_globals}"  "++++++++++"
      print_msg "REGISTRY" "from: ${globals_registry_file_app}" "$(cat ${globals_registry_file_app} )"
      print_msg "CURRENT CONTAINER BARE" "$(get_globals_app)"

      return 1

    fi

  fi

  print_soft_ok "All global variable instances reported in registry (executable)."

  return 0

}


