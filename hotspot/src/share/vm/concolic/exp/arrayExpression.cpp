#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

ArrayExpression::ArrayExpression(sym_rid_t array_id, Expression *index_exp,
                                 Expression *value_exp, bool is_load, BasicType type)
    : _index_exp(index_exp), _value_exp(value_exp), _is_load(is_load) {
  int ret = sprintf(_arr_str, "A%c_%lu", type2char(type), array_id);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");

  if (_index_exp) {
    _index_exp->inc_ref();
  }
  if (_value_exp) {
    _value_exp->inc_ref();
  }
}

ArrayExpression::~ArrayExpression() {
  Expression::gc(_index_exp);
  Expression::gc(_value_exp);
}

void ArrayExpression::print() {
  if (_is_load) {
    tty->print("(select %s ", _arr_str);
  } else {
    tty->print("(store %s ", _arr_str);
  }
  _index_exp->print();
  tty->print(" ");
  _value_exp->print();
  tty->print(")");
  // z3-friend    /\ 
  // human-friend \/
  // tty->print("%s[", _arr_str);
  // _index_exp->print();
  // tty->print(_is_load ? "] -> " : "] <- ");
  // _value_exp->print();
}

#endif