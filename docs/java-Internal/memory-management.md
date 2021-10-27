# How to allocate memory?

By default, hotspot disable global new operator to prevent memory leak.

For fast development, we re-allow global new operator in [this file](hotspot/src/share/vm/memory/allocation.cpp).

TODO: integrate the memory allocations used by our code into hotspot's memory mangement