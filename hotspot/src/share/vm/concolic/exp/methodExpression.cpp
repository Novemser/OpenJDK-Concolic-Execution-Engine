#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "utilities/ostream.hpp"

MethodExpression::MethodExpression(const std::string &holder, const std::string &method, exp_list_t &param_list,
                                   Expression *res_exp, bool ignorable)
    : _name(holder + ' ' + method), _res_exp(res_exp), _ignorable(ignorable) {

  // all exp must be not null

  _param_list.swap(param_list);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _param_list[i];
    if (exp) {
      exp->inc_ref();
    }
  }
  if (_res_exp) {
    _res_exp->inc_ref();
  }
}

MethodExpression::~MethodExpression() {
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression::gc(_param_list[i]);
  }
  Expression::gc(_res_exp);
}

void MethodExpression::print() {
  if(_ignorable) {
    tty->print("$");
  }
  tty->print("(f %s ", _name.c_str());
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    tty->print(" ,");
    Expression::print_on_maybe_null(_param_list[i]);
  }
  tty->print(" ");
  Expression::print_on_maybe_null(_res_exp);
  tty->print(")");
}

#endif