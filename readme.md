# Build Tutorials

cd to the project's root directory

./scripts/configure.sh
make all -j CONF=linux-x86_64-normal-zero-slowdebug

source ./scripts/init_env.sh

cd exmaple
../scripts/myjavac Example.java

../scripts/myjava Example
or
../scripts/myjavagdb Example