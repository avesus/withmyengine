#!/bin/bash

# See https://stackoverflow.com/a/34208365/84661
# Full path of the current script
THIS=`readlink -f "${BASH_SOURCE[0]}" 2>/dev/null||echo $0`
# The directory where current script resides
DIR=`dirname "${THIS}"`

DIR_WAS=$PWD
cd $DIR
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake $DIR/../../platform/linux/ || exit $?
make || exit $?
cd $DIR_WAS
exec $DIR/withmyengine "$@"

