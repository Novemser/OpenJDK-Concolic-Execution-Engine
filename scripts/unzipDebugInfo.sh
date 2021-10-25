if [ $# -eq 0 ]; then
    unzip -o build/linux-x86_64-normal-server-slowdebug/jdk/lib/amd64/server/libjvm.diz -d build/linux-x86_64-normal-server-slowdebug/jdk/lib/amd64/server/
else
    unzip -o build/linux-x86_64-normal-server-release/jdk/lib/amd64/server/libjvm.diz -d build/linux-x86_64-normal-server-release/jdk/lib/amd64/server/
fi
