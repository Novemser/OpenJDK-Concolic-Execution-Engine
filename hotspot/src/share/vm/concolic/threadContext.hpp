#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "concolic/shadowStack.hpp"
#include "runtime/handles.hpp"

#include <map>

class ThreadContext {
  typedef std::map<sym_oid_t, SymbolicObject *> SymStore;

private:
  JavaThread* _thread;
  SymStore _sym_objs;
  ShadowStack _s_stack;

  sym_oid_t _sym_oid_counter;
public:
  ThreadContext(JavaThread* jt);
  ~ThreadContext();

  void symbolize(Handle handle);

  SymbolicObject *alloc_sym_obj(oop obj);
  SymbolicObject *get_sym_obj(sym_oid_t sym_oid);

private:
  void symbolize_recursive(SymbolicObject *sym_obj, oop obj);

  void set_sym_obj(sym_oid_t sym_oid, SymbolicObject *sym_obj) {
    _sym_objs.insert(std::make_pair(sym_oid, sym_obj));
  }

  sym_oid_t get_next_sym_oid() {
    sym_oid_t ret = _sym_oid_counter++;
    assert(ret < MAX_SYM_OID, "sym_oid limitted");
    return ret;
  }

public:
  void print();
  void print_stack_trace();
};

#endif

#endif // SHARE_VM_CONCOLIC_THREADCONTEXT_HPP