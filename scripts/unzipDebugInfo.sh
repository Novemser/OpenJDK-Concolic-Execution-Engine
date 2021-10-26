#!/bin/sh

unzip -o $MYJAVA_HOME/lib/amd64/server/libjvm.diz -d $MYJAVA_HOME/lib/amd64/server/
# unzip -o $MYJAVA_BUILD/hotspot/linux_amd64_zero/debug/libjvm.diz -d $MYJAVA_BUILD/hotspot/linux_amd64_zero/debug/

# unzip -o $MYJAVA_HOME/bin/java.diz -d ./
# if [ $# -eq 0 ]; then
#     unzip -o build/linux-x86_64-normal-server-slowdebug/jdk/lib/amd64/server/libjvm.diz -d build/linux-x86_64-normal-server-slowdebug/jdk/lib/amd64/server/
# else
#     unzip -o build/linux-x86_64-normal-server-release/jdk/lib/amd64/server/libjvm.diz -d build/linux-x86_64-normal-server-release/jdk/lib/amd64/server/
# fi
