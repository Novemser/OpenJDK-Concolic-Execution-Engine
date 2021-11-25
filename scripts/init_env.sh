#!/bin/sh

# export MYJAVA_VARIENT="linux-x86_64-normal-zero-release"
export MYJAVA_VARIENT="linux-x86_64-normal-zero-release"
export MYJAVA_BUILD="$(pwd)/build"
export MYJAVA_HOME="$MYJAVA_BUILD/$MYJAVA_VARIENT/jdk"

export SERVER_RELEASE_JAVA_HOME="$MYJAVA_BUILD/linux-x86_64-normal-server-release/jdk"
export ZERO_RELEASE_JAVA_HOME="$MYJAVA_BUILD/linux-x86_64-normal-zero-release/jdk"