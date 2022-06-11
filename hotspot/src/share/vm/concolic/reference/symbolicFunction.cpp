//
// Created by gansen on 6/11/22.
//

#include "symbolicFunction.hpp"

SymbolicFunction::SymbolicFunction(SymbolicFunction::FunctionType type, Expression *parameterExpr) : _type(type),
                                                                                                     _parameterExpr(
                                                                                                         parameterExpr) {
  guarantee(_parameterExpr, "should not be null");
  _parameterExpr->inc_ref();
}

void SymbolicFunction::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("SymbolicFunction");
  writer.Key("_func_type");
  switch (_type) {
    case TO_TIMESTAMP:
      writer.String("TO_TIMESTAMP");
      break;
    default:
      tty->print_cr("type not implemented %d", _type);
      ShouldNotCallThis();
  }

  writer.Key("_exp");
  if (_parameterExpr != NULL) {
    _parameterExpr->serialize(writer);
  } else {
    writer.Null();
  }
}

SymbolicFunction::~SymbolicFunction() {
  Expression::gc(_parameterExpr);
}
