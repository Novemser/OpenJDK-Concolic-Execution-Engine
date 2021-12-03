#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicTimestamp.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

const char *SymTimestamp::TYPE_NAME = "java/sql/Timestamp";
method_set_t SymTimestamp::symbolized_methods = init_symbolized_methods();

method_set_t SymTimestamp::init_symbolized_methods() {
  method_set_t m_set;
  m_set.insert("<init>");
  return m_set;
}

SymTimestamp::SymTimestamp(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}
SymTimestamp::SymTimestamp(sym_rid_t sym_rid, Expression *exp) : SymInstance(sym_rid), _exp(exp) {}

SymTimestamp::~SymTimestamp() {
  Expression::gc(_exp);
}

Expression *SymTimestamp::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    ResourceMark rm;
    //TODO: get double value from Timestamp obj;
    ShouldNotCallThis();
  }
  return exp;
}

bool SymTimestamp::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
    int offset = handle.get_callee_local_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();
    Method *callee_method = handle.get_callee_method();

    need_symbolize = true;
  } else {
    tty->print_cr("Timestamp Unhandled method:");
    tty->print_cr(callee_name.c_str());
    //ShouldNotCallThis();
  }

  return need_symbolize;
}

Expression *SymTimestamp::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp;
  ShouldNotCallThis();
  return exp;
}

void SymTimestamp::print() {
  tty->print_cr("SymTimestamp: ");
  _exp->print();
  tty->print("\n");
}

#endif