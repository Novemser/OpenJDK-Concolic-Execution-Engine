#include "concolic/threadContext.hpp"

#ifdef ENABLE_CONCOLIC

ThreadContext::ThreadContext() {
    _sym_oid_counter = 0;
}

#endif