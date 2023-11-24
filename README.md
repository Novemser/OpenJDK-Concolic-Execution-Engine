![logo](./WeBridge.png)

[![pipeline status](https://ipads.se.sjtu.edu.cn:1312/ipads-storage/codebase/wbridge/openjdk8-webridge/badges/master/pipeline.svg)](https://ipads.se.sjtu.edu.cn:1312/ipads-storage/codebase/wbridge/openjdk8-webridge/-/commits/master)

# WeBridge OpenJDK Concolic Execution Engine

Implementation of a concolic execution engine on top of OpenJDK 8.

The engine currently supports interpreter mode execution.

# Prerequisites
We have tested the code in the following environment:

Operating system: Ubuntu 16.04

GCC/G++: 6.5.0

# How to build
First install necessary dependencies.
```shell
./scripts/install_dependencies.sh
```
Next, configure the project.
```shell
./scripts/configure.sh
```
Then, build the project.
```shell
make all -j CONF=linux-x86_64-normal-zero-release
```

The built concolic execution JDK path will be in `./build/linux-x86_64-normal-zero-release/images/j2sdk-image`. Next, set this path as your default `JAVA_HOME` to use the concolic execution JDK.

# Quick start

Refer to the `./concolic-test` directory to see examples on how to run Java programs with the concolic execution engine.
