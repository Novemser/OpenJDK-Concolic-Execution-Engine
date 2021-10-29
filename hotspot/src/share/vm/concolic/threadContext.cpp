
#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"
#include "concolic/fieldTraverser.hpp"

ThreadContext::ThreadContext() { _sym_oid_counter = 0; }

void ThreadContext::symbolize(Handle handle) {
  SymbolicObject *sym_obj = this->alloc_sym_obj(handle());
  this->symbolize_recursive(sym_obj, handle());
}

SymbolicObject *ThreadContext::alloc_sym_obj(oop obj) {
  sym_oid_t sym_oid = get_next_sym_oid();

  obj->set_sym_oid(sym_oid);

  SymbolicObject *sym_obj = new SymbolicObject(sym_oid);
  this->set_sym_obj(sym_oid, sym_obj);

  return sym_obj;
}

SymbolicObject *ThreadContext::get_sym_obj(sym_oid_t sym_oid) {
  SymbolicObject *ret = _sym_objs[sym_oid];
  assert(ret != NULL, "null sym obj?");
  return ret;
}

void ThreadContext::symbolize_recursive(SymbolicObject *sym_obj, oop obj) {
  tty->print("ThreadContext::symbolize_recursive\n");

  SimpleFieldPrinter field_printer(obj);
  field_printer.do_recursive();

  FieldSymbolizer field_symbolzier(obj, *this);
  field_symbolzier.do_recursive();
}

#endif