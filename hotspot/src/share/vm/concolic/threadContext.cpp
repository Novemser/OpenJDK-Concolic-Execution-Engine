
#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"
#include "concolic/fieldTraverser.hpp"
#include "utilities/vmError.hpp"

ThreadContext::ThreadContext(JavaThread *jt) : _thread(jt), _s_stack(jt) {
  _sym_oid_counter = 0;
}

ThreadContext::~ThreadContext() {
  SymStore::iterator sym_iter;
  for (sym_iter = _sym_objs.begin(); sym_iter != _sym_objs.end(); ++sym_iter) {
    delete sym_iter->second;
  }
  _sym_objs.clear();
  _sym_oid_counter = 0;
}

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

  // SimpleFieldPrinter field_printer(obj);
  // field_printer.do_recursive();

  FieldSymbolizer field_symbolzier(obj, *this);
  field_symbolzier.do_recursive();
}

void ThreadContext::print() {
  SymStore::iterator sym_iter;
  for (sym_iter = _sym_objs.begin(); sym_iter != _sym_objs.end(); ++sym_iter) {
    sym_iter->second->print();
  }
}

void ThreadContext::print_stack_trace() {
  ZeroStack *stack = _thread->zero_stack();
  static char buf[O_BUFLEN];
  VMError::print_stack_trace(tty, _thread, buf, sizeof(buf));
}

#endif