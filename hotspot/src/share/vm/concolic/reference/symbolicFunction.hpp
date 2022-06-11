//
// Created by gansen on 6/11/22.
//

#ifndef JDK8_CDB_SYMBOLICFUNCTION_HPP
#define JDK8_CDB_SYMBOLICFUNCTION_HPP

#ifdef ENABLE_WEBRIDGE
#include "concolic/exp/expression.hpp"

class SymbolicFunction : public Expression {
public:
  enum FunctionType {
    TO_TIMESTAMP
  };

private:
  FunctionType _type;
  Expression *_parameterExpr;
protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

public:
  SymbolicFunction(FunctionType type, Expression *parameterExpr);
  ~SymbolicFunction();
};
#endif

#endif //JDK8_CDB_SYMBOLICFUNCTION_HPP
