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

class FieldSymbolicExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  char _sym_str[EXP_NAME_LENGTH];

public:
  FieldSymbolicExpression(char *sym_name, int field_index);

public:
  void print();
};

class OpSymbolicExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  Expression *_left;
  Expression *_right;
  SymbolicOp _op;

public:
  OpSymbolicExpression(Expression *l, Expression *r,
                       SymbolicOp op, bool cmp = true);

public:
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_EXP_EXPRESSION_HPP