#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicBigDecimal.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"

const char *SymBigDecimal::TYPE_NAME = "java/math/BigDecimal";
method_set_t SymBigDecimal::symbolized_methods = init_symbolized_methods();

method_set_t SymBigDecimal::init_symbolized_methods() {
  method_set_t m_set;
  m_set.insert("<init>");
//  m_set.insert("add");
//  m_set.insert("subtract");
//  m_set.insert("divide");
//  m_set.insert("multiply");
  return m_set;
}

SymBigDecimal::SymBigDecimal(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}

SymBigDecimal::~SymBigDecimal() {
  Expression::gc(_exp);
}

Expression *SymBigDecimal::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    ResourceMark rm;
    //TODO: get double value from BigDecimal obj;
    ShouldNotCallThis();
  }
  return exp;
}

bool SymBigDecimal::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
    int offset = handle.get_callee_local_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();
    Method *callee_method = handle.get_callee_method();

    need_symbolize = true;
  } else {
    tty->print_cr("BigDecimal Unhandled method:");
    tty->print_cr(callee_name.c_str());
    ShouldNotCallThis();
  }

  return need_symbolize;
}

Expression *SymBigDecimal::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp;
  ShouldNotCallThis();
//  if (callee_name == "add") {
//    oop left_obj = handle.get_param<oop>(0);
//    oop right_obj = handle.get_param<oop>(1);
//    if (left_obj->is_symbolic() || right_obj->is_symbolic()) {
//      oop res_obj = handle.get_result<oop>(T_OBJECT);
//      Expression *left_exp = SymBigDecimal::get_exp_of(left_obj);
//      Expression *right_exp = SymBigDecimal::get_exp_of(right_obj);
//      SymBigDecimal *sym_res = (SymBigDecimal *)ConcolicMngr::ctx->get_or_alloc_sym_inst(res_obj);
//      exp = new OpSymExpression(left_exp, right_exp, op_add, true);
//      sym_res->set_ref_exp(exp);
//    }
//  } else {
//    ShouldNotCallThis();
//  }
  return exp;
}

void SymBigDecimal::print() {
  tty->print_cr("SymBigDecimal: ");
  _exp->print();
  tty->print("\n");
}

#endif