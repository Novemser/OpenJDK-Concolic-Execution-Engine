//
// Created by gansen on 6/7/22.
//

#include "placeholderExpression.hpp"
#ifdef ENABLE_WEBRIDGE
std::map<PlaceholderExpression::Function, int> PlaceholderExpression::functionInvCounter;

PlaceholderExpression::PlaceholderExpression(PlaceholderExpression::Function type) : _type(type), _count(0) {
  if (functionInvCounter.find(type) == functionInvCounter.end()) {
    functionInvCounter[type] = 0;
  }

  functionInvCounter[type]++;
  _count = functionInvCounter[type];
}

void PlaceholderExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("PlaceholderExpression");
  writer.Key("_place_holder_func");
  switch (_type) {
    case PlaceholderExpression::currentTimeMills: {
      writer.String("currentTimeMills");
      break;
    }
    case PlaceholderExpression::uuidToString: {
      writer.String("uuidToString");
      break;
    }
    default:
      ShouldNotReachHere();
  }
  writer.Key("_count");
  writer.Int(_count);
}

void PlaceholderExpression::print() {
  tty->print_cr("PlaceHolderExp:%u.%d", _type, _count);
}
#endif