#!/bin/bash/

GNUFILES_DIR="$( cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )"/.gnufiles
###plots in pdf format for paper 
function plotE288 {

local dir="$1"
local key1="$2"
local key2="$3"

if [[ ! -d  "${dir}" ]]; then
  echo "Directory ${dir} not found"
  return 1
fi

local id="$(basename "${dir}")"

local gnufname="plot_fit_E288_linear.gnu"

mkdir -p "$dir/plots/"

if [[ -f "$dir/plots/$gnufname" ]];then

  echo "Plotting from existing script '$dir/plots/$gnufname'"

  gnuplot "$dir/plots/$gnufname"

  return 0

fi

echo "Generating new script '$dir/plots/$gnufname'"

cat "${GNUFILES_DIR}/$gnufname" | \
\
sed -E "s|GNUFILES_DIR|${GNUFILES_DIR}|g" | \
\
sed -E "s|^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+|\1\"$dir\"|g" | \
\
sed -E "s|^([[:space:]]*ID[[:space:]]*=[[:space:]]*).+|\1\"$id\"|g" | \
\
sed -E "s|^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+|\1\"$key1\"|g" | \
\
sed -E "s|^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+|\1\"$key2\"|g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

}

function plotE605 {

local dir="$1"
local key1="$2"
local key2="$3"

if [[ ! -d  "${dir}" ]]; then
  echo "Directory ${dir} not found"
  return 1
fi

local id="$(basename "${dir}")"

local gnufname="plot_fit_E605_linear.gnu"

mkdir -p "$dir/plots/"

if [[ -f "$dir/plots/$gnufname" ]];then

  echo "Plotting from existing script '$dir/plots/$gnufname'"

  gnuplot "$dir/plots/$gnufname"

  return 0

fi

echo "Generating new script '$dir/plots/$gnufname'"

cat "${GNUFILES_DIR}/$gnufname" | \
\
sed -E "s|GNUFILES_DIR|${GNUFILES_DIR}|g" | \
\
sed -E "s|^([[:space:]]*DIR[[:space:]]*=[[:space:]]*).+|\1\"$dir\"|g" | \
\
sed -E "s|^([[:space:]]*ID[[:space:]]*=[[:space:]]*).+|\1\"$id\"|g" | \
\
sed -E "s|^([[:space:]]*key1[[:space:]]*=[[:space:]]*).+|\1\"$key1\"|g" | \
\
sed -E "s|^([[:space:]]*key2[[:space:]]*=[[:space:]]*).+|\1\"$key2\"|g"  \
\
> "$dir/plots/$gnufname"

gnuplot "$dir/plots/$gnufname"

}


