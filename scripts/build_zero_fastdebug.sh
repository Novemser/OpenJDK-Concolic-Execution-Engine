#!/bin/sh

make -j hotspot jdk CONF=linux-x86_64-normal-zero-fastdebug

unzip -o $ZERO_FASTDEBUG_JAVA_HOME/lib/amd64/server/libjvm.diz -d $ZERO_FASTDEBUG_JAVA_HOME/lib/amd64/server/