#ifndef SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "utilities/debug.hpp"

#include <stdio.h>

class Expression {
private:
  uint _ref_count;

public:
  virtual void print();
  inline void inc_ref() { _ref_count += 1; }

protected:
  Expression() : _ref_count(0) {}
};

class FieldSymExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  char _str[EXP_NAME_LENGTH];

public:
  FieldSymExpression(char *sym_name, int field_index);

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