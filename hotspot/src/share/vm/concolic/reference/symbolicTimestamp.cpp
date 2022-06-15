#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicTimestamp.hpp"
#include "concolic/reference/symbolicFunction.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

const char *SymTimestamp::TYPE_NAME = "java/sql/Timestamp";
method_set_t SymTimestamp::symbolized_methods = init_symbolized_methods();

method_set_t SymTimestamp::init_symbolized_methods() {
  method_set_t m_set;
//  m_set.insert("<init>");
//  m_set.insert("getTime");
//  m_set.insert("getNanos");
//  m_set.insert("setTime");
//  m_set.insert("setNanos");
  return m_set;
}

void SymTimestamp::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymTimestamp::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymTimestamp::TYPE_NAME, finish_method_helper);
}

SymTimestamp::SymTimestamp(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL), _exp_converted(NULL) {}
SymTimestamp::SymTimestamp(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid), _exp(new InstanceSymbolExp(obj)), _exp_converted(NULL) {
  guarantee(obj != NULL, "should not be null");
  Klass *tsClz = obj->klass();
  guarantee(tsClz->name() == vmSymbols::java_sql_Timestamp(), "should equals");

  fieldDescriptor fd_fastTime;
  tsClz->find_field(
      vmSymbols::fastTime(),
      vmSymbols::long_signature(),
      &fd_fastTime
  );
  fieldDescriptor fd_nanos;
  tsClz->find_field(
      vmSymbols::nanos(),
      vmSymbols::int_signature(),
      &fd_nanos
  );
  _fastTimeFldOffset = fd_fastTime.offset();
  _nanosFldOffset = fd_nanos.offset();
}

SymTimestamp::~SymTimestamp() {
  Expression::gc(_exp);
  std::map<int, Expression*>::iterator iter = _internal_fields.begin();
  for (; iter != _internal_fields.end(); iter++) {
    Expression::gc(iter->second);
  }
  Expression::gc(_exp_converted);
}

Expression *SymTimestamp::get_exp_of(oop obj) {
  ResourceMark rm;
  Klass* tsClz = obj->klass();
  assert(tsClz->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    fieldDescriptor fd_fastTime;
    tsClz->find_field(
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

bool SymTimestamp::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
//    if (handle.general_check_param_symbolized()) {
    need_symbolize = true;
//      handle.general_prepare_param();
//    }
  } else {
//    handle.get_callee_method()->print_name(tty);
//    tty->print_cr("unhandled by SymTimestamp:");
  }

  return need_symbolize;
}

Expression *SymTimestamp::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;
//  if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
//    ShouldNotCallThis();
//  }
  return exp;
}

void SymTimestamp::print() {
  tty->print_cr("SymTimestamp: ");
  _exp->print();
  tty->print("\n");
}

void SymTimestamp::init_sym_exp(int field_offset, Expression *exp) {
  guarantee(exp != NULL, "should not be null");
  exp->inc_ref();

  if (_internal_fields.find(field_offset) != _internal_fields.end()) {
    Expression::gc(_internal_fields[field_offset]);
  }
  _internal_fields[field_offset] = exp;
}

Expression *SymTimestamp::get(int field_offset) {
  return _internal_fields[field_offset];
}

void SymTimestamp::set_sym_exp(int field_offset, Expression *exp) {
  if (exp) {
    exp->inc_ref();
  }
  Expression::gc(_internal_fields[field_offset]);
  _internal_fields[field_offset] = exp;
}

Expression *SymTimestamp::get_ref_exp() {
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

void SymTimestamp::set_timestamp_symbolic(oop tsOOp, std::string name) {
  std::string fastTimeName = name + "#fastTime";
  std::string nanosName = name + "#nanos";
  init_sym_exp(_fastTimeFldOffset, new SymbolExpression(fastTimeName.c_str(), fastTimeName.length()));
  init_sym_exp(_nanosFldOffset, new SymbolExpression(nanosName.c_str(), nanosName.length()));
}

#endif