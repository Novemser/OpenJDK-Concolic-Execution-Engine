#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

ArrayExpression::ArrayExpression(sym_rid_t array_id, Expression *index_exp,
                                 Expression *value_exp, bool is_load)
    : _index_exp(index_exp), _value_exp(value_exp), _is_load(is_load) {
  int ret = sprintf(_arr_str, "A_%lu", array_id);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");

  if (_index_exp) {
    _index_exp->inc_ref();
  }
  if (_value_exp) {
    _value_exp->inc_ref();
  }
}

ArrayExpression::~ArrayExpression() {
  if (_index_exp && _index_exp->dec_ref()) {
    delete _index_exp;
  }
  if (_value_exp && _value_exp->dec_ref()) {
    delete _value_exp;
  }
}

void ArrayExpression::print() {
  tty->print("%s[", _arr_str);
  _index_exp->print();
  tty->print(_is_load ? "] -> " : "] <- ");
  _value_exp->print();
}

#endif