#!/bin/sh

DEBUG_FLAGS="-DENABLE_CONCOLIC -DONELINE -DCONCOLIC_JDBC -DCONCOLIC_DEBUG -DENABLE_WEBRIDGE"
RELEASE_FLAGS="-DENABLE_CONCOLIC -DONELINE -DCONCOLIC_JDBC -DENABLE_WEBRIDGE"
# LDFLAGS="-L/home/gansen/softwares/libXtst-1.2.1/lib"
# X_INCLUDES="/home/gansen/Code/libXtst-1.2.1/include"
# WITH_CUPS="/home/gansen/softwares/cups-2.2.10"
# JTREG_HOME="/home/gansen/Code/jtreg/build/images/jtreg"
# FLAGS="-DENABLE_CONCOLIC -DCONCOLIC_DEBUG"
# FLAGS="-DENABLE_CONCOLIC"

bash ./configure                    \
    --with-debug-level=release

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=release    \
    --with-extra-cflags="$RELEASE_FLAGS"    \
    --with-extra-cxxflags="$RELEASE_FLAGS"

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=fastdebug    \
    --with-extra-cflags="$DEBUG_FLAGS"    \
    --with-extra-cxxflags="$DEBUG_FLAGS"

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=slowdebug    \
    --with-extra-cflags="$DEBUG_FLAGS"    \
    --with-extra-cxxflags="$DEBUG_FLAGS"
