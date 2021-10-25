cmake_minimum_required(VERSION 3.7)
PROJECT(Closure-receiver)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        hotspot/src/share/tools
        hotspot/src/share/vm
        hotspot/src/cpu/x86/vm
        hotspot/src/os/linux/vm
        hotspot/src/os/posix/vm
        hotspot/src/os_cpu/linux_x86/vm
        hotspot/src/share/vm/precompiled
        hotspot/src/share/vm/prims
        jdk/src/solaris/bin
        jdk/src/share/native/common
        build/linux-x86_64-normal-server-release/hotspot/linux_amd64_compiler2/generated
        build/linux-x86_64-normal-server-release/jdk/gensrc_headers
        build/linux-x86_64-normal-server-release/hotspot/dist/include

)


file(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "*.hpp"
        "*.c"
        "*.h")

list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/cpu/ppc/.*")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/cpu/sparc/.*")
list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/cpu/zero/.*")



add_executable(ClosureReceiver ${SOURCE_FILES})

add_definitions(-DTARGET_COMPILER_gcc)

add_definitions(-DTARGET_ARCH_x86)
add_definitions(-DLINUX)
add_definitions(-DTARGET_OS_FAMILY_linux)
add_definitions(-DINCLUDE_ALL_GCS)
add_definitions(-DTARGET_ARCH_MODEL_x86_64)
add_definitions(-DTARGET_OS_ARCH_linux_x86)
add_definitions(-DCOMPILER2)
add_definitions(-DCOMPILER1)
add_definitions(-DASSERT)
add_definitions(-DAMD64)
