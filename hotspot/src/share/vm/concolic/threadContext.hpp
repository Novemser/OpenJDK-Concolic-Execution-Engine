#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include <map>

class ThreadContext {
typedef ulong id_t;

private:
    id_t _id_counter;
    std::map<uintptr_t, uintptr_t> store;

public:
    ThreadContext();
};

#endif

#endif // SHARE_VM_CONCOLIC_THREADCONTEXT_HPP