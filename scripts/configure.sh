#!/bin/sh

FLAGS="-DENABLE_CONCOLIC -DCONCOLIC_DEBUG -DONELINE -DCONCOLIC_JDBC "
# FLAGS="-DENABLE_CONCOLIC -DCONCOLIC_DEBUG"
# FLAGS="-DENABLE_CONCOLIC"

bash ./configure                    \
    --with-debug-level=release

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=release    \
    --with-extra-cflags="$FLAGS"    \
    --with-extra-cxxflags="$FLAGS"

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=fastdebug    \
    --with-extra-cflags="$FLAGS"    \
    --with-extra-cxxflags="$FLAGS"

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=slowdebug    \
    --with-extra-cflags="$FLAGS"    \
    --with-extra-cxxflags="$FLAGS"
