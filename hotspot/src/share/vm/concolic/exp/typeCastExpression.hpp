//
// Created by gansen on 6/28/22.
//

#ifndef JDK8_CDB_TYPECASTEXPRESSION_HPP
#define JDK8_CDB_TYPECASTEXPRESSION_HPP
#ifdef ENABLE_WEBRIDGE

#include "concolic/exp/expression.hpp"
#include "concolic/SymbolicOp.hpp"
#include "concolic/defs.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"
#include "symbolExpression.hpp"

#include <map>
#include <stdio.h>
#include <vector>

class TypeCastExpression : public SymbolExpression {
private:
  Expression *_exp_to_cast;
public:
  TypeCastExpression(Expression *exp) : _exp_to_cast(exp) {
    if (exp) {
      exp->inc_ref();
    }
  }

  ~TypeCastExpression() {
    Expression::gc(_exp_to_cast);
  }

  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

  virtual std::string get_name();
};

#endif
#endif //JDK8_CDB_TYPECASTEXPRESSION_HPP
