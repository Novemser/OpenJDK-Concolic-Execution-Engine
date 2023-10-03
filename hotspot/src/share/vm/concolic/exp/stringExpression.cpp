#ifdef ENABLE_CONCOLIC

#include "concolic/exp/stringExpression.hpp"
#include "utilities/ostream.hpp"
#include <concolic/utils.hpp>

OpStrExpression::OpStrExpression(const std::string &method,
                                 exp_list_t &param_list)
    : _name(method), _param_cache(NULL), _is_not(false) {
  // all exp must be not null

  _param_list.swap(param_list);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _param_list[i];
    if (exp) {
      exp->inc_ref();
    }
  }
}

OpStrExpression::OpStrExpression(const std::string &method,
                                 exp_list_t &param_list, bool is_not)
    : _name(method), _param_cache(NULL), _is_not(is_not) {
  // all exp must be not null

  _param_list.swap(param_list);
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _param_list[i];
    if (exp) {
      exp->inc_ref();
    }
  }
}

OpStrExpression::OpStrExpression(const std::string &method, Expression *exp)
    : _name(method), _param_cache(NULL), _is_not(false) {
  if (exp) {
    exp->inc_ref();
  }
  _param_list.push_back(exp);
}

OpStrExpression::~OpStrExpression() {
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    Expression::gc(_param_list[i]);
  }
  delete[] _param_cache;
}

void OpStrExpression::print() {
  tty->print("(s %s ", _name.c_str());
  int size = _param_list.size();
  for (int i = 0; i < size; ++i) {
    tty->print(" ,");
    Expression::print_on_maybe_null(_param_list[i]);
  }
  tty->print(" )");
}

OpStrExpression *OpStrExpression::to_string(Expression *exp) {
  return new OpStrExpression("toString", exp);
}

void OpStrExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("OpStrExpression");
  writer.Key("_name");
  writer.String(_name.c_str());
  writer.Key("_is_not");
  writer.Bool(_is_not);

  writer.Key("_param_list");
  if (_param_cache == NULL) {
    using namespace rapidjson;
    StringBuffer s;
    Writer<StringBuffer> cache_writer(s);
    cache_writer.StartArray();
    for (size_t index = 0; index < _param_list.size(); ++index) {
      Expression *expr = _param_list[index];
      if (expr) {
        expr->serialize(cache_writer);
      } else {
        cache_writer.Null();
      }
    }
    cache_writer.EndArray();
    _param_cache = new char[s.GetLength() + 1];
    strcpy(_param_cache, s.GetString());
    assert(strlen(_param_cache) == s.GetLength(), "should equals");
  }

  writer.RawValue(_param_cache, strlen(_param_cache), rapidjson::kArrayType);
}

const exp_list_t &OpStrExpression::get_param_list() const {
  return _param_list;
}

std::string OpStrExpression::get_name() {
  std::string name;
  name += _name;
  name += "(";
  for (size_t index = 0; index < _param_list.size(); ++index) {
    Expression *expr = _param_list[index];
    if (expr) {
      name += expr->get_name();
    } else {
      name += "null";
    }
    if (index != _param_list.size() - 1) {
      name += ",";
    }
  }
  name += ")";
  return name;
}

StringSymbolExp::StringSymbolExp(sym_rid_t sym_rid) {
  ResourceMark rm;
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', T_OBJECT, "'String'");
  ss.print("%lu", sym_rid);
  _type = vmSymbols::java_lang_String()->as_C_string();
  this->finalize(ss.size());
}

StringSymbolExp::StringSymbolExp(sym_rid_t sym_rid, std::string name) {
  ResourceMark rm;
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', T_OBJECT, "'String'");
  ss.print("%s", name.c_str());
  _type = vmSymbols::java_lang_String()->as_C_string();
  this->finalize(ss.size());
}

ConStringSymbolExp::ConStringSymbolExp(oop obj) {
  ResourceMark rm;
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'Y', T_OBJECT, "'String'");
  _type = vmSymbols::java_lang_String()->as_C_string();
  ss.print("%s", OopUtils::java_string_to_c(obj));
  this->finalize(ss.size());
}

ConStringSymbolExp::ConStringSymbolExp(const std::string &str) {
  ResourceMark rm;
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'Y', T_OBJECT, "'String'");
  _type = vmSymbols::java_lang_String()->as_C_string();
  ss.print("%s", str.c_str());
  this->finalize(ss.size());
}

void ConStringSymbolExp::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("ConStringSymbolExp");
  writer.Key("_java_type");
  writer.String(_type.c_str());
  writer.Key("_exp");
  writer.String(_str.c_str());
}

#endif