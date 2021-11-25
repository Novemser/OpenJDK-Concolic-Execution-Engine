#!/bin/sh

make -j hotspot jdk CONF=linux-x86_64-normal-zero-slowdebug

unzip -o $ZERO_SLOWDEBUG_JAVA_HOME/lib/amd64/server/libjvm.diz -d $ZERO_SLOWDEBUG_JAVA_HOME/lib/amd64/server/