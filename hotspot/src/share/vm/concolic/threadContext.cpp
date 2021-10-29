#include "concolic/threadContext.hpp"

#ifdef ENABLE_CONCOLIC

ThreadContext::ThreadContext() { _sym_oid_counter = 0; }

void ThreadContext::symbolize(Handle handle) {
  sym_oid_t sym_oid = get_next_sym_oid();
  assert(sym_oid < MAX_SYM_OID, "sym_oid limitted");

  handle()->set_sym_oid(sym_oid);

  SymbolicObject *sym_obj = new SymbolicObject(sym_oid);
  _sym_objs.insert(std::make_pair(sym_oid, sym_obj));

  sym_obj->symbolize(handle);
}

#endif