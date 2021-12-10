#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "utilities/ostream.hpp"

MethodExpression::MethodExpression(const std::string &holder,
                                   const std::string &method,
                                   exp_list_t &param_list)
    : _name(holder + '.' + method) {

  // all exp must be not null

  _param_list.swap(param_list);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _param_list[i];
    if (exp) {
      exp->inc_ref();
    }
  }
}

MethodExpression::~MethodExpression() {
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression::gc(_param_list[i]);
  }
}

void MethodExpression::print() {
  tty->print("(f %s ", _name.c_str());
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression::print_on_maybe_null(_param_list[i]);
    tty->print(",");
  }
  tty->print(")");
}

Expression *MethodExpression::get_return_pc(const std::string &holder, const std::string &method,
                                 exp_list_t &param_list, Expression *res_exp) {
  return new OpSymExpression(new MethodExpression(holder, method, param_list), res_exp, op_eq);
}


#endif