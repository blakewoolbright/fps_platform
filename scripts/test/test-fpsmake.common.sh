#!/bin/bash

SCRIPT_NAME=$(basename "$(test -L "$0" && readlink -e "$0" || echo "$0")")
SCRIPT_DIR=$(dirname "$(readlink -e ${0})")

source ${SCRIPT_DIR}/../fpsmake.common 

root_dir=$(fps__get_root_dir)
build_dir=$(fps__get_build_dir)

echo "fps__get_root_dir   :: ${root_dir}"
echo "fps__get_build_dir  :: ${build_dir}"

TESTPATH="/home/blake/bin"

fps__path_append TESTPATH /a_bin /a_sbin
fps__path_append TESTPATH /a_bin /a_sbin
echo "fps__path_append     :: ${TESTPATH}"

fps__path_prepend TESTPATH /p_bin /p_sbin
echo "fps__path_prepend    :: ${TESTPATH}"

CPU_CORES=$(fps__count_cpu_cores)
echo "fps__count_cpu_cores :: ${CPU_CORES}"

