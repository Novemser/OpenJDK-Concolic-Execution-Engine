#!/bin/sh

# export MYJAVA_VARIENT="linux-x86_64-normal-zero-release"
export MYJAVA_VARIENT="linux-x86_64-normal-zero-slowdebug"
export MYJAVA_BUILD="$(pwd)/build/$MYJAVA_VARIENT"
export MYJAVA_HOME="$MYJAVA_BUILD/jdk"