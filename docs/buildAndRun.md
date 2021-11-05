# Preparation

Firstly, you need to reach the project's root directory

## How to build

`
./scripts/configure.sh
`

`
make all -j CONF=linux-x86_64-normal-zero-slowdebug
`

## How to set environment variables used by this project

`
source ./scripts/init_env.sh
`

## How to run and debug a java file?
`
cd exmaple
`

`
../scripts/myjavac Example.java
`

`
../scripts/myjava Example
or
../scripts/myjavagdb Example
`
