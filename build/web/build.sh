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
rm CMakeCache.txt
cmake $DIR/../../platform/web/ || exit $?
make || exit $?
cd $DIR_WAS
cp $DIR/../../platform/web/index.html $DIR/index.html
cd $DIR
exec python -m SimpleHTTPServer
# exec $DIR/withmyengine "$@"

