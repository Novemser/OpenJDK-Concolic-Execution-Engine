#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicObject.hpp"
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

  void symbolize(Handle handle) {
    sym_oid_t sym_oid = get_next_sym_oid();
    assert(sym_oid < MAX_SYM_OID, "sym_oid limitted");

    handle()->set_sym_oid(sym_oid);

    SymbolicObject *sym_obj = new SymbolicObject(sym_oid);

    _sym_objs.insert(std::make_pair(sym_oid, sym_obj));
  }

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