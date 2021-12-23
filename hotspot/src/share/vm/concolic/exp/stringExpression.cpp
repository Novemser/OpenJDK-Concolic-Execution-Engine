#ifdef ENABLE_CONCOLIC

#include "concolic/exp/stringExpression.hpp"
#include "utilities/ostream.hpp"
#include <concolic/utils.hpp>

OpStrExpression::OpStrExpression(const std::string &method,
                                 exp_list_t &param_list)
    : _name(method) {
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

OpStrExpression::~OpStrExpression() {
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression::gc(_param_list[i]);
  }
}

void OpStrExpression::print() {
  tty->print("(s %s ", _name.c_str());
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    tty->print(",");
    Expression::print_on_maybe_null(_param_list[i]);
  }
  tty->print(" )");
}

ConStringSymbolExp::ConStringSymbolExp(oop obj) {
  ResourceMark rm;
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'Y', T_OBJECT, "'String'");
  ss.print("%s", OopUtils::java_string_to_c(obj));
  this->finalize(ss.size());
}

ConStringSymbolExp::ConStringSymbolExp(const std::string &str) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'Y', T_OBJECT, "'String'");
  ss.print("%s", str.c_str());
  this->finalize(ss.size());
}

#endif