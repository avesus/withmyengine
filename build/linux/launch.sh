#!/bin/bash

# Full path of the current script
THIS=`readlink -f "${BASH_SOURCE[0]}" 2>/dev/null||echo $0`

# The directory where current script resides
DIR=`dirname "${THIS}"`

# Exit codes:
# 126: permission denied
# 127: command not found
# 128: invalid argument to exit
$DIR/build.sh -abc -darg1 -e arg2 --foo-opt=arg3; ec=$?; if [ $ec -gt 128 ] && [ $ec -lt 255 ]; then echo "process terminated by signal $(($ec-128))"; else echo "exit code was $ec"; fi

