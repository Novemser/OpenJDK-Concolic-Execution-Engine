
#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"
#include "concolic/fieldTraverser.hpp"
#include "utilities/vmError.hpp"

ThreadContext::ThreadContext(JavaThread *jt) : _thread(jt), _s_stack(jt) {
  init_sym_oid_counter();
  init_sym_tmp_id_counter();
}

ThreadContext::~ThreadContext() {
  SymStore::iterator sym_iter;
  for (sym_iter = _sym_objs.begin(); sym_iter != _sym_objs.end(); ++sym_iter) {
    delete sym_iter->second;
  }
  _sym_objs.clear();
  _sym_tmp_exps.clear();
  init_sym_oid_counter();
  init_sym_tmp_id_counter();
}

void ThreadContext::symbolize(Handle handle) {
  SymbolicObject *sym_obj = this->alloc_sym_obj(handle());
  this->symbolize_recursive(sym_obj, handle());
}

SymbolicObject *ThreadContext::get_or_alloc_sym_obj(oop obj) {
  if (obj->is_symbolic()) {
    return this->get_sym_obj(obj->get_sym_oid());
  } else {
    return this->alloc_sym_obj(obj);
  }
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

sym_tmp_id_t ThreadContext::get_next_sym_tmp_id(SymbolicExpression *sym_exp) {
  sym_oid_t sym_tmp_id = _sym_tmp_id_counter++;
  _sym_tmp_exps.push_back(sym_exp);
  assert(_sym_tmp_exps.size() == sym_tmp_id + 1, "sanity check");
  assert(sym_tmp_id < MAX_SYM_OID, "sym_tmp_id limitted");
  return sym_tmp_id;
}

void ThreadContext::print() {
  for (SymStore::iterator sym_iter = _sym_objs.begin();
       sym_iter != _sym_objs.end(); ++sym_iter) {
    tty->print_cr("- sym_obj[%lu]:", sym_iter->first);
    sym_iter->second->print();
  }

  int size = _sym_tmp_exps.size();
  for (int i = 1; i < size; ++i) {
    tty->print_cr("- sym_tmp_exp[%d]:", i);
    _sym_tmp_exps[i]->print();
  }

  _path_condition.print();
}

void ThreadContext::print_stack_trace() {
  ZeroStack *stack = _thread->zero_stack();
  static char buf[O_BUFLEN];
  VMError::print_stack_trace(tty, _thread, buf, sizeof(buf));
}

#endif