#include "concolic/threadContext.hpp"

#ifdef ENABLE_CONCOLIC

ThreadContext::ThreadContext() {
    _id_counter = 0;
}

#endif