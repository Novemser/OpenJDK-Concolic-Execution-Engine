#ifndef SHARE_VM_CONCOLIC_EXP_METHODEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_METHODEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"

#include <stdio.h>
#include <string>
#include <vector>

class MethodExpression : public Expression {
  std::string _name;
  exp_list_t _param_list;
  Expression* _res_exp;
  // ignore this expression!
  bool _ignorable;

public:
  MethodExpression(const std::string &holder, const std::string &method, exp_list_t &param_list,
                   Expression *res_exp, bool ignorable = false);
  ~MethodExpression();

public:
  void print();

protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);
};

#endif

#endif // SHARE_VM_CONCOLIC_EXP_METHODEXPRESSION_HPP