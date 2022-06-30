//
// Created by gansen on 6/7/22.
//

#ifndef JDK8_CDB_PLACEHOLDEREXPRESSION_HPP
#define JDK8_CDB_PLACEHOLDEREXPRESSION_HPP
#ifdef ENABLE_WEBRIDGE
#include "concolic/exp/expression.hpp"

class PlaceholderExpression : public Expression {
public:
  enum Function {
    currentTimeMills,
    uuidToString
  };

  PlaceholderExpression(Function type);

protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

public:
  virtual void print();

  virtual std::string get_name();

private:
  static std::map<Function, int> functionInvCounter; // record stack trace
  Function _type;
  int _count;
};

#endif
#endif //JDK8_CDB_PLACEHOLDEREXPRESSION_HPP
