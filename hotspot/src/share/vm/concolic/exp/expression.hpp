#ifndef SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

#include <stdio.h>

class Expression {
private:
  uint _ref_count;

public:
  static ulong total_count;

  virtual ~Expression() { total_count--; }
  virtual void print();

  inline void inc_ref() { _ref_count += 1; }
  inline bool dec_ref() { return --_ref_count == 0; }
  inline bool able_to_gc() { return _ref_count == 0; }

protected:
  Expression() : _ref_count(0) { total_count++; }
};

class FieldSymExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  char _str[EXP_NAME_LENGTH];

public:
  FieldSymExpression(char *sym_name, int field_index);
  ~FieldSymExpression();

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
  static const int EXP_NAME_LENGTH = 16;

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

#endif

#endif // SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP