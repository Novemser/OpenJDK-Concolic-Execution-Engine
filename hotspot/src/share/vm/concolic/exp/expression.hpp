#ifndef SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "concolic/defs.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"
#include "webridge/utils/rapidjson/writer.h"

#include <map>
#include <cstdio>
#include <vector>
class Expression {
private:
  uint _ref_count;
  std::string _java_code_position;

public:
  static ulong total_count;

  static void gc(Expression *exp) {
    if (exp && exp->dec_ref()) {
      delete exp;
    }
  }

public:
  virtual ~Expression() { total_count--; }
  virtual void print();
  void print_cr();
  virtual bool is_op_str_expression() { return false; }

  static void print_on_maybe_null(Expression *exp) {
    if (exp) {
      exp->print();
    } else {
      tty->print("U");
    }
  }

  inline void inc_ref() { _ref_count += 1; }
  inline bool dec_ref() { return --_ref_count == 0; }
  inline bool able_to_gc() { return _ref_count == 0; }

  void set_java_code_position(const std::string &javaCodePosition);

  const std::string &getJavaCodePosition() const;

protected:
  Expression();

protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) const = 0;

public:
  void serialize(rapidjson::Writer<rapidjson::StringBuffer> &writer) const {
    writer.StartObject();
    if (!_java_code_position.empty()) {
      writer.Key("_code_position");
      writer.String(_java_code_position.c_str());
    }
    serialize_internal(writer);
    writer.EndObject();
  }
};

class OpSymExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  Expression *_left;
  Expression *_right;
  SymbolicOp _op;

public:
  OpSymExpression(Expression *l, Expression *r, SymbolicOp op, bool cmp = true);
  OpSymExpression(Expression *r, SymbolicOp op);
  ~OpSymExpression();

public:
  void print();

protected:
  void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
};

class ConExpression : public Expression {
  static const int EXP_NAME_LENGTH = 32;

private:
  char _str[EXP_NAME_LENGTH];

public:
  ConExpression(jboolean b);
  ConExpression(jbyte c);
  ConExpression(jchar b);
  ConExpression(jint i);
  ConExpression(jshort s);
  ConExpression(jlong l);
  ConExpression(jfloat f);
  ConExpression(jdouble d);

public:
  void print();

  void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
};

class ArrayExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  char _arr_str[EXP_NAME_LENGTH];
  bool _is_load;
  Expression *_index_exp;
  Expression *_value_exp;

public:
  ArrayExpression(sym_rid_t array_id, Expression *index_exp,
                  Expression *value_exp, bool is_load, BasicType type);
  ~ArrayExpression();

public:
  void print();

  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) const;
};

typedef std::vector<Expression *> exp_list_t;
typedef std::map<int, Expression *> exp_map_t;

#endif

#endif // SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP