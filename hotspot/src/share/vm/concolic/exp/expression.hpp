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
  uint _unique_id;
  std::string _java_code_position;
  static std::set<Expression *> deleted;
  static std::set<Expression *> keep;

public:
  static ulong total_count;
  static ulong unique_id;
  static void gc(Expression *exp);
  static void init_gc_helper();
  static void finalize_dangling_objects();

public:
  virtual ~Expression();
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

  inline int get_unique_id() { return _unique_id; }
protected:
  Expression();

protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) = 0;

public:
  void __attribute__((optimize("O0"))) serialize(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
    writer.StartObject();
    if (!_java_code_position.empty()) {
      writer.Key("_code_position");
      writer.String(_java_code_position.c_str());
    }
    serialize_internal(writer);
    writer.EndObject();
  }

  virtual std::string get_name();
};

class OpSymExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  Expression *_left;
  Expression *_right;
  SymbolicOp _op;
  char* _left_cache;
  char* _right_cache;

public:
  OpSymExpression(Expression *l, Expression *r, SymbolicOp op, bool cmp = true);
  OpSymExpression(Expression *r, SymbolicOp op);
  ~OpSymExpression();
  virtual std::string get_name();

public:
  void print();

protected:
  void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);
};

class ConExpression : public Expression {
  static const int EXP_NAME_LENGTH = 32;

private:
  char _str[EXP_NAME_LENGTH];
  std::string _type;

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

  void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);
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

  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

  virtual std::string get_name();
  static Expression *get_exp_of(oop obj);
};

typedef std::vector<Expression *> exp_list_t;
typedef std::map<int, Expression *> exp_map_t;

#endif

#endif // SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP