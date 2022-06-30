#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

ArrayExpression::ArrayExpression(sym_rid_t array_id, Expression *index_exp,
                                 Expression *value_exp, bool is_load, BasicType type)
    : _index_exp(index_exp), _value_exp(value_exp), _is_load(is_load) {
  int ret = sprintf(_arr_str, "A%c_%lu", type2char(type), array_id);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");

  if (_index_exp) {
    _index_exp->inc_ref();
  }
  if (_value_exp) {
    _value_exp->inc_ref();
  }
}

ArrayExpression::~ArrayExpression() {
  Expression::gc(_index_exp);
  Expression::gc(_value_exp);
}

void ArrayExpression::print() {
  if (_is_load) {
    tty->print("(select %s ", _arr_str);
  } else {
    tty->print("(store %s ", _arr_str);
  }
  _index_exp->print();
  tty->print(" ");
  _value_exp->print();
  tty->print(")");
  // z3-friend    /\ 
  // human-friend \/
  // tty->print("%s[", _arr_str);
  // _index_exp->print();
  // tty->print(_is_load ? "] -> " : "] <- ");
  // _value_exp->print();
}

void ArrayExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("_array_expr");
  writer.Key("_arr_str");
  writer.String(_arr_str);
  writer.Key("_is_load");
  writer.Bool(_is_load);
  writer.Key("_index_exp");
  if (_index_exp) {
    _index_exp->serialize(writer);
  } else {
    writer.Null();
  }
  writer.Key("_value_exp");
  if (_value_exp) {
    _value_exp->serialize(writer);
  } else {
    writer.Null();
  }
}

std::string ArrayExpression::get_name() {
  std::string name = "";
  if (_is_load) {
    name = "(select " + std::string(_arr_str) + " ";
  } else {
    name = "(store " + std::string(_arr_str) + " ";
  }
  if (_index_exp) {
    name += _index_exp->get_name();
  } else {
    name += "null";
  }
  name += "_";
  if (_value_exp) {
    name += _value_exp->get_name();
  } else {
    name += "null";
  }

  return name;
}

#endif