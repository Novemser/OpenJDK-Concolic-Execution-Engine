#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/fieldTraverser.hpp"
#include "concolic/reference/symbolicObject.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "utilities/vmError.hpp"

ThreadContext::ThreadContext(JavaThread *jt) : _thread(jt), _s_stack(jt) {
  init_sym_rid_counter();
  init_sym_tmp_id_counter();
}

ThreadContext::~ThreadContext() {
  for (SymStore::iterator iter = _sym_refs.begin(); iter != _sym_refs.end();
       ++iter) {
    delete iter->second;
  }
  _sym_refs.clear();

  int size = _sym_tmp_exps.size();
  for (int i = 1; i != size; ++i) {
    Expression *sym_exp = _sym_tmp_exps[i];
    if (sym_exp && sym_exp->able_to_gc()) {
      delete sym_exp;
    }
  }
  _sym_tmp_exps.clear();

  _path_condition.gc();
}

void ThreadContext::symbolize(Handle handle) {
  if (handle()->is_instance()) {
    this->alloc_sym_inst(handle());
    this->symbolize_recursive(handle());
  } else if (handle()->is_array()) {
    SymArr *sym_arr = this->alloc_sym_array((arrayOop)handle());
    sym_arr->set_length_exp(new SymbolExpression(sym_arr->get_sym_rid(), 0, 0));
    /**
     * Currently, we do not call symbolize_recursive for array
     * Because symbolic array always return a new symbolic expression.
     */
  } else {
    assert(false, "unhandled");
  }
}

SymInstance *ThreadContext::get_or_alloc_sym_inst(oop obj) {
  if (obj->is_symbolic()) {
    return this->get_sym_inst(obj->get_sym_rid());
  } else {
    return this->alloc_sym_inst(obj);
  }
}

SymInstance *ThreadContext::alloc_sym_inst(oop obj) {
  sym_rid_t sym_rid = get_next_sym_rid();
  obj->set_sym_rid(sym_rid);
  SymInstance *sym_inst;

  Klass *klass = obj->klass();
  if (klass->name()->equals("java/lang/String")) {
    sym_inst = new SymString(sym_rid);
  } else {
    sym_inst = new SymObj(sym_rid);
  }

  this->set_sym_ref(sym_rid, sym_inst);

  return sym_inst;
}

SymInstance *ThreadContext::get_sym_inst(sym_rid_t sym_rid) {
  SymInstance *ret = (SymInstance *)_sym_refs[sym_rid];
  /**
   * When this assertion is broken,
   * it means that the target object is not initialized with NULL_SYM_RID
   */
  assert(ret != NULL, "null sym obj?");
  return ret;
}

SymArr *ThreadContext::get_or_alloc_sym_array(arrayOop array,
                                              Expression *length_exp) {
  if (array->is_symbolic()) {
    return this->get_sym_array(array->get_sym_rid());
  } else {
    return this->alloc_sym_array(array, length_exp);
  }
}

SymArr *ThreadContext::alloc_sym_array(arrayOop array, Expression *length_exp) {
  sym_rid_t sym_rid = get_next_sym_rid();
  array->set_sym_rid(sym_rid);

  // if (length_exp == NULL) {
  //   length_exp = new ConExpression(array->length());
  // }

  SymArr *sym_arr = new SymArr(sym_rid, length_exp);
  this->set_sym_ref(sym_rid, sym_arr);

  this->record_path_condition(
      new ArrayInitExpression(sym_arr->get_sym_rid(), array));

  return sym_arr;
}

SymArr *ThreadContext::get_sym_array(sym_rid_t sym_rid) {
  SymArr *ret = (SymArr *)_sym_refs[sym_rid];
  /**
   * When this assertion is broken,
   * it means that the target object is not initialized with NULL_SYM_RID
   */
  assert(ret != NULL, "null sym obj?");
  return ret;
}

void ThreadContext::symbolize_recursive(oop obj) {
  tty->print("ThreadContext::symbolize_recursive\n");

  // SimpleFieldPrinter field_printer(obj);
  // field_printer.do_recursive();

  FieldSymbolizer field_symbolzier(obj, *this);
  field_symbolzier.do_recursive();
  // OopTraverser oop_traverser;
  // tty->print_cr(CL_CYAN"================================================================="CNONE);
  // obj->oop_iterate(&oop_traverser);
  // tty->print_cr(CL_CYAN"================================================================="CNONE);
}

sym_tmp_id_t ThreadContext::get_next_sym_tmp_id(Expression *sym_exp) {
  sym_rid_t sym_tmp_id = _sym_tmp_id_counter++;
  _sym_tmp_exps.push_back(sym_exp);
  assert(_sym_tmp_exps.size() == sym_tmp_id + 1, "sanity check");
  assert(sym_tmp_id < MAX_SYM_RID, "sym_tmp_id limitted");
  return sym_tmp_id;
}

void ThreadContext::detach_tmp_exp(sym_tmp_id_t sym_tmp_id) {
  _sym_tmp_exps[sym_tmp_id] = NULL;
}

void ThreadContext::print() {
  for (SymStore::iterator sym_iter = _sym_refs.begin();
       sym_iter != _sym_refs.end(); ++sym_iter) {
    tty->print_cr("- sym_inst[%lu]:", sym_iter->first);
    sym_iter->second->print();
  }

  int size = _sym_tmp_exps.size();
  for (int i = 1; i < size; ++i) {
    Expression *sym_exp = _sym_tmp_exps[i];
    if (sym_exp) {
      tty->print_cr("<< sym_tmp_exp[%d]:", i);
      sym_exp->print_cr();
    }
  }

  _path_condition.print();
}

void ThreadContext::print_stack_trace() {
  ZeroStack *stack = _thread->zero_stack();
  static char buf[O_BUFLEN];
  VMError::print_stack_trace(tty, _thread, buf, sizeof(buf));
}

#endif