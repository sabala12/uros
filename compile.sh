#!/usr/bin/env bash
#===============================================================================
#
#          FILE: compile.sh
# 
#         USAGE: ./compile.sh 
# 
#   DESCRIPTION: 
# 
#       OPTIONS: ---
#  REQUIREMENTS: ---
#          BUGS: ---
#         NOTES: ---
#        AUTHOR: ERAN SABALA (sabalah21@gmail.com), 
#  ORGANIZATION: 
#       CREATED: 27/05/17 11:17:54
#      REVISION:  ---
#===============================================================================

set -o nounset                              # Treat unset variables as an error
set -o errexit                              # Exit on command failure

# Set magic variables for current file & dir
__dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
__file="${__dir}/$(basename "${BASH_SOURCE[0]}")"
__base="$(basename ${__file} .sh)"
__root="$(cd "$(dirname "${__dir}")" && pwd)" # <-- change this as it depends on your app

if [ ! -d "${__dir}/build" ]; then
	mkdir "${__dir}/build"
fi

cd "${__dir}/build"
cmake ..
make

if [ $? -eq 0 ]; then
	make iso
fi
