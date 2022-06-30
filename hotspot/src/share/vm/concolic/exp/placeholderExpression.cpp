//
// Created by gansen on 6/7/22.
//

#include "placeholderExpression.hpp"
#ifdef ENABLE_WEBRIDGE
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

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

std::string PlaceholderExpression::get_name() {
  std::string name = "PlaceholderExpression_";
  switch (_type) {
    case PlaceholderExpression::currentTimeMills: {
      name += "currentTimeMills";
      break;
    }
    case PlaceholderExpression::uuidToString: {
      name += "uuidToString";
      break;
    }
    default:
      ShouldNotReachHere();
  }
  name += SSTR("_" << _count);
  return name;
}

#endif