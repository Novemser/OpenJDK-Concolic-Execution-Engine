#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"

SymString::SymString(sym_rid_t sym_rid) : SymInstance(sym_rid) {}

SymString::~SymString() {
  if (_exp && _exp->dec_ref()) {
    delete _exp;
  }
}

Expression *SymString::get(int field_offset) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  return _exp;
}

void SymString::init_sym_exp(int field_offset) {
  init_sym_exp(field_offset,
               new SymbolExpression(this->get_sym_rid(), field_offset));
}

void SymString::init_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  exp->inc_ref();
  _exp = exp;
}

void SymString::set_sym_exp(int field_offset, Expression *exp) {
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

void SymString::print() {}

#endif