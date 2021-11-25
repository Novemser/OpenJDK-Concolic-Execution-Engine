#!/bin/sh

FLAGS="-DENABLE_CONCOLIC -DCONCOLIC_DEBUG -DONELINE -DCONCOLIC_JDBC "
# FLAGS="-DENABLE_CONCOLIC -DCONCOLIC_DEBUG"
# FLAGS="-DENABLE_CONCOLIC"

bash ./configure                    \
    --with-jvm-variants=zero        \
    --with-debug-level=release    \
    --with-extra-cflags="$FLAGS"    \
    --with-extra-cxxflags="$FLAGS"
