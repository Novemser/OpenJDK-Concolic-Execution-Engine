#ifndef SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "concolic/defs.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

#include <map>
#include <stdio.h>
#include <vector>
class Expression {
private:
  uint _ref_count;

public:
  static ulong total_count;

  virtual ~Expression() { total_count--; }
  virtual void print();
  void print_cr();

  inline void inc_ref() { _ref_count += 1; }
  inline bool dec_ref() { return --_ref_count == 0; }
  inline bool able_to_gc() { return _ref_count == 0; }

protected:
  Expression() : _ref_count(0) { total_count++; }
};

class SymbolExpression : public Expression {
public:
  static const int NULL_INDEX = -1;
  static sym_rid_t sym_method_count;

private:
  static const int EXP_NAME_LENGTH = 16;

  char _str[EXP_NAME_LENGTH];

public:
  SymbolExpression(sym_rid_t sym_rid, int field_index, BasicType type);
  SymbolExpression(sym_rid_t sym_arr_oid, int arr_version, int load_count,
                   BasicType type);
  SymbolExpression(const char *prefix, sym_rid_t id);
  SymbolExpression();
  ~SymbolExpression();

public:
  void print();
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
};

class ConExpression : public Expression {
  static const int EXP_NAME_LENGTH = 32;

private:
  char _str[EXP_NAME_LENGTH];

public:
  ConExpression(jint i);
  ConExpression(jlong l);
  ConExpression(jfloat f);
  ConExpression(jdouble d);

public:
  void print();
};

class ArrayExpression : public Expression {
  static const int EXP_NAME_LENGTH = 7;

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
};

typedef std::vector<Expression *> exp_list_t;
typedef std::map<int, Expression *> exp_map_t;

#endif

#endif // SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP