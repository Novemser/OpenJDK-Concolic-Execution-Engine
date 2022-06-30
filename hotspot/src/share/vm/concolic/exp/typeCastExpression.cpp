//
// Created by gansen on 6/28/22.
//

#include "typeCastExpression.hpp"

void TypeCastExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("TypeCastExpression");
  writer.Key("_java_type");
  writer.String(_type.c_str());
  writer.Key("_exp");
  _exp_to_cast->serialize(writer);
}

std::string TypeCastExpression::get_name() {
  std::string name = "CAST_AS_" + _type + "_";
  return name + _exp_to_cast->get_name();
}
