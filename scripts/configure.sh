#!/bin/sh

bash ./configure                                \
    --with-jvm-variants=zero                    \
    --with-debug-level=slowdebug                \
    --with-extra-cflags="-DENABLE_CONCOLIC" \
    --with-extra-cxxflags="-DENABLE_CONCOLIC"
