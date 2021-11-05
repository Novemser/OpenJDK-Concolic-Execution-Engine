#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

SelectExpression::SelectExpression(sym_oid_t array_id, Expression *index_exp,
                                   Expression *value_exp)
    : _index_exp(index_exp), _value_exp(value_exp) {
  int ret = sprintf(_arr_str, "A_%lu", array_id);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");

  if (_index_exp) {
    _index_exp->inc_ref();
  }
  if (_value_exp) {
    _value_exp->inc_ref();
  }
}

SelectExpression::~SelectExpression() {
  if (_index_exp && _index_exp->dec_ref()) {
    delete _index_exp;
  }
  if (_value_exp && _value_exp->dec_ref()) {
    delete _value_exp;
  }
}

void SelectExpression::print() {
  tty->print("%s[", _arr_str);
  _index_exp->print();
  tty->print("] == ");
  _value_exp->print();
}

#endif