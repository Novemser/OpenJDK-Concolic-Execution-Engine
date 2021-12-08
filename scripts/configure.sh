#!/bin/sh

DEBUG_FLAGS="-DENABLE_CONCOLIC -DONELINE -DCONCOLIC_JDBC -DCONCOLIC_DEBUG"
RELEASE_FLAGS="-DENABLE_CONCOLIC -DONELINE -DCONCOLIC_JDBC"
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
