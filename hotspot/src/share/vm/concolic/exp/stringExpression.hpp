#ifndef SHARE_VM_CONCOLIC_EXP_STRINGEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_STRINGEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <string>

class StringExpression : public Expression {
  std::string _str;

public:
  StringExpression(const char *str) : _str(str) {}
  ~StringExpression() {}

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_EXP_STRINGEXPRESSION_HPP