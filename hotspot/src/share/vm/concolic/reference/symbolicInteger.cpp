#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicInteger.hpp"

const char *SymInteger::TYPE_NAME = "java/lang/Integer";
sym_rid_t SymInteger::sym_int_count = 0;

SymInteger::SymInteger(sym_rid_t sym_rid) : SymInstance(sym_rid) {
  // new SymbolExpression("INT", sym_int_count++)
  // _ref_exp->inc_ref();
}

SymInteger::~SymInteger() {
  if (_exp && _exp->dec_ref()) {
    delete _exp;
  }
  // assert(_ref_exp, "not NULL");
  // if (_ref_exp && _ref_exp->dec_ref()) {
  //   delete _ref_exp;
  // }
}

Expression *SymInteger::get(int field_offset) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  return _exp;
}

void SymInteger::init_sym_exp(int field_offset) {
  init_sym_exp(field_offset,
               new SymbolExpression(this->get_sym_rid(), field_offset));
}

void SymInteger::init_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  exp->inc_ref();
  _exp = exp;
}

void SymInteger::set_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  if (_exp && _exp->dec_ref()) {
    delete _exp;
  }

  _exp = exp;
  if (_exp) {
    _exp->inc_ref();
  }
}

void SymInteger::print() {
  tty->print_cr("SymInteger: ");
  _exp->print();
  tty->print("\n");
}

Expression *SymInteger::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    exp = new ConExpression(OopUtils::java_int_to_c(obj));
  }
  return exp;
}

#endif