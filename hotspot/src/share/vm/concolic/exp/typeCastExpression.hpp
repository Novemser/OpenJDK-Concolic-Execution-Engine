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
    if (_exp_to_cast) {
      _exp_to_cast->inc_ref();
    }
  }

  ~TypeCastExpression() {
    if (_exp_to_cast) {
      _exp_to_cast->dec_ref();
    }
  }

  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);
};

#endif
#endif //JDK8_CDB_TYPECASTEXPRESSION_HPP
