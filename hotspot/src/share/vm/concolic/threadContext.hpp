#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "runtime/handles.hpp"

#include <map>

class ThreadContext {
  static const int MAX_SYM_OID = 10000;
  typedef std::map<sym_oid_t, SymbolicObject *> SymStore;

private:
  sym_oid_t _sym_oid_counter;
  SymStore _sym_objs;

public:
  ThreadContext();
  ~ThreadContext() { reset(); }

  void symbolize(Handle handle);

  void reset() {
    SymStore::iterator sym_iter;
    for (sym_iter = _sym_objs.begin(); sym_iter != _sym_objs.end();
         ++sym_iter) {
      delete sym_iter->second;
    }
    _sym_objs.clear();
    _sym_oid_counter = 0;
  }

private:
  sym_oid_t get_next_sym_oid() { return _sym_oid_counter++; }
};

#endif

#endif // SHARE_VM_CONCOLIC_THREADCONTEXT_HPP