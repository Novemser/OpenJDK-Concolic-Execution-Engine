#!/bin/sh

make -j hotspot jdk CONF=linux-x86_64-normal-server-fastdebug

unzip -o $MYJAVA_BUILD/linux-x86_64-normal-server-fastdebug/lib/amd64/server/libjvm.diz -d $MYJAVA_BUILD/linux-x86_64-normal-server-fastdebug/lib/amd64/server/