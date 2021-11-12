#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "utilities/ostream.hpp"

MethodExpression::MethodExpression(const char *holder, const char *method,
                                   ParamList &param_list, Expression *res_exp)
    : _res_exp(res_exp) {

  int ret = sprintf(_method_str, "%s.%s", holder, method);
  assert(ret <= METHOD_NAME_LENGTH, "METHOD_NAME_LENGTH exceeded!");

  // all exp must be not null

  _param_list.swap(param_list);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    _param_list[i]->inc_ref();
  }

  _res_exp->inc_ref();
}

MethodExpression::~MethodExpression() {
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _param_list[i];
    if (exp && exp->dec_ref()) {
      delete exp;
    }
  }

  if (_res_exp->dec_ref()) {
    delete _res_exp;
  }
}

void MethodExpression::print() {
  tty->print_cr("(%s: ", _method_str);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    _param_list[i]->print();
  }
  tty->print(") -> ");
  _res_exp->print();
}

#endif