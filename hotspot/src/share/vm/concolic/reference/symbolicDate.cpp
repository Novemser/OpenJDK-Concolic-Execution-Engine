//
// Created by gansen on 6/15/22.
//

#ifdef ENABLE_WEBRIDGE

#include "symbolicDate.hpp"
#include "concolic/reference/symbolicFunction.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

const char *SymbolicDate::TYPE_NAME = "java/sql/Date";

SymbolicDate::SymbolicDate(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL), _exp_converted(NULL) {
}

SymbolicDate::SymbolicDate(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid), _exp(new InstanceSymbolExp(obj)),
                                                         _exp_converted(NULL) {
  guarantee(obj != NULL, "should not be null");
  Klass *tsClz = obj->klass();
  guarantee(tsClz->name()->equals(TYPE_NAME), "should equals");

  fieldDescriptor fd_fastTime;
  tsClz->find_field(
      vmSymbols::fastTime(),
      vmSymbols::long_signature(),
      &fd_fastTime
  );
  _fastTimeFldOffset = fd_fastTime.offset();
}

SymbolicDate::~SymbolicDate() {
  Expression::gc(_exp);
  std::map<int, Expression *>::iterator iter = _internal_fields.begin();
  for (; iter != _internal_fields.end(); iter++) {
    Expression::gc(iter->second);
  }
  Expression::gc(_exp_converted);
}

void SymbolicDate::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymbolicDate::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymbolicDate::TYPE_NAME, finish_method_helper);
}

Expression *SymbolicDate::get_exp_of(oop obj) {
  ResourceMark rm;
  Klass *dtClz = obj->klass();

  assert(dtClz->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    fieldDescriptor fd_fastTime;
    dtClz->find_field(
        vmSymbols::fastTime(),
        vmSymbols::long_signature(),
        &fd_fastTime
    );
    exp = new SymbolicFunction(SymbolicFunction::TO_TIMESTAMP,
                               new OpSymExpression(
                                   new ConExpression(obj->long_field(fd_fastTime.offset())), new ConExpression(1000),
                                   op_div
                               ));
  }
  return exp;
}

void SymbolicDate::init_sym_exp(int field_offset, Expression *exp) {
  guarantee(exp != NULL, "should not be null");
  exp->inc_ref();

  if (_internal_fields.find(field_offset) != _internal_fields.end()) {
    Expression::gc(_internal_fields[field_offset]);
  }
  _internal_fields[field_offset] = exp;
}

void SymbolicDate::print() {

}

Expression *SymbolicDate::get(int field_offset) {
  return _internal_fields[field_offset];
}

void SymbolicDate::set_sym_exp(int field_offset, Expression *exp) {
  if (exp) {
    exp->inc_ref();
  }
  Expression::gc(_internal_fields[field_offset]);
  _internal_fields[field_offset] = exp;
}

bool SymbolicDate::invoke_method_helper(MethodSymbolizerHandle &handle) {
  return false;
}

Expression *SymbolicDate::finish_method_helper(MethodSymbolizerHandle &handle) {
  return NULL;
}

Expression *SymbolicDate::get_ref_exp() {
  // return fast time
  Expression *exp = _internal_fields[_fastTimeFldOffset];
  if (exp == NULL) return exp;
  if (_exp_converted != NULL) return _exp_converted;
  Expression::gc(_exp_converted);
  // FROM_UNIXTIME is base on seconds, so we need to convert million seconds to seconds
  _exp_converted = new SymbolicFunction(SymbolicFunction::TO_TIMESTAMP,
                                        new OpSymExpression(
                                            exp, new ConExpression(1000), op_div
                                        ));
  _exp_converted->inc_ref();
//  tty->print_cr("new op_div location:%p", _exp_converted);
  return _exp_converted;
}

void SymbolicDate::set_date_symbolic(oop tsOOp, std::string name) {
  std::string fastTimeName = name + "#fastTime";
  init_sym_exp(_fastTimeFldOffset, new SymbolExpression(fastTimeName.c_str(), fastTimeName.length()));
}

#endif
