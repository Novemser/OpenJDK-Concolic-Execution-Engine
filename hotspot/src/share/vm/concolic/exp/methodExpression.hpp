#ifndef SHARE_VM_CONCOLIC_EXP_METHODEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_METHODEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"

#include <stdio.h>
#include <vector>

class MethodExpression : public Expression {
public:
  typedef std::vector<Expression *> ParamList;

private:
  static const int METHOD_NAME_LENGTH = 16;

private:
  char _method_str[METHOD_NAME_LENGTH];
  ParamList _param_list;
  Expression *_res_exp;

public:
  MethodExpression(const char *holder, const char *method,
                   ParamList &param_list, Expression *res_exp);
  ~MethodExpression();

public:
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_EXP_METHODEXPRESSION_HPP