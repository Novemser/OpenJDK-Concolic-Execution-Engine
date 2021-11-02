#ifndef SHARE_VM_CONCOLIC_SYMBOLICEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "utilities/debug.hpp"

#include <stdio.h>

class SymbolicExpression {
  static const int EXP_NAME_LENGTH = 16;

private:
  union {
    char exp[EXP_NAME_LENGTH];
    struct {
      SymbolicExpression *left;
      SymbolicExpression *right;
    };
  } _data;
  uint _ref_count;
  SymbolicOp _op;
  bool _is_leaf;

public:
  SymbolicExpression(char *sym_name, int field_index);
  SymbolicExpression(SymbolicExpression *l, SymbolicExpression *r,
                     SymbolicOp op);

  inline void inc_ref() { _ref_count += 1; }

  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SYMBOLICEXPRESSION_HPP