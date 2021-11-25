#!/bin/sh

make -j hotspot jdk CONF=linux-x86_64-normal-zero-release

unzip -o $ZERO_RELEASE_JAVA_HOME/lib/amd64/server/libjvm.diz -d $ZERO_RELEASE_JAVA_HOME/lib/amd64/server/