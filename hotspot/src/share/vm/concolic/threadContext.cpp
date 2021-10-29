#include "concolic/threadContext.hpp"

#ifdef ENABLE_CONCOLIC

ThreadContext::ThreadContext() {
    _sym_oid_counter = 0;
}

void ThreadContext::symbolize(Handle handle) {
  // TODO: it's temporal to use 0 as sym_oid
  SymbolicObject so(0);
  so.symbolize(handle);
}

#endif