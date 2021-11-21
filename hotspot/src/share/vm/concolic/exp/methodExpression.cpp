#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "utilities/ostream.hpp"

MethodExpression::MethodExpression(const std::string &holder,
                                   const std::string &method,
                                   exp_list_t &param_list, Expression *res_exp)
    : _name(holder + '.' + method), _res_exp(res_exp) {

  // all exp must be not null

  _param_list.swap(param_list);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    _param_list[i]->inc_ref();
  }

  /**
   * It seems that methods returning void does not need to be symbolized?
   */

  if (res_exp) {
    _res_exp->inc_ref();
  }
}

MethodExpression::~MethodExpression() {
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _param_list[i];
    if (exp && exp->dec_ref()) {
      delete exp;
    }
  }

  if (_res_exp && _res_exp->dec_ref()) {
    delete _res_exp;
  }
}

void MethodExpression::print() {
  tty->print_cr("%s(", _name.c_str());
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    tty->print("  ");
    _param_list[i]->print();
  }
  tty->print(") -> ");
  if (_res_exp) {
    _res_exp->print();
  } else {
    tty->print("VOID");
  }
}

#endif