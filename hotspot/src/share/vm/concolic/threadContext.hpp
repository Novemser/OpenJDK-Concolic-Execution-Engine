#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicState.hpp"
#include "runtime/handles.hpp"

#include <map>

class ThreadContext {
  typedef std::map<oop *, SymbolicState *> SymStore;

private:
  sym_id_t _id_counter;
  SymStore _sym_store;

public:
  ThreadContext();
  ~ThreadContext() { reset(); }

  void symbolize(Handle handle) {
    sym_id_t sym_id = get_next_sym_id();
		assert(sym_id < 10000, "sym_id limitted");
    SymbolicState *sym_state = new SymbolicState(sym_id, handle);

    _sym_store.insert(std::make_pair(handle.raw_value(), sym_state));
  }

  void reset() {
    SymStore::iterator sym_iter;
    for (sym_iter = _sym_store.begin(); sym_iter != _sym_store.end();
         ++sym_iter) {
      delete sym_iter->second;
    }
    _sym_store.clear();
    _id_counter = 0;
  }

private:
  sym_id_t get_next_sym_id() { return _id_counter++; }
};

#endif

#endif // SHARE_VM_CONCOLIC_THREADCONTEXT_HPP