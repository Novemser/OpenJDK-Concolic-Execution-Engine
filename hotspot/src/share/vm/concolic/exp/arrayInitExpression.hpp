#ifndef SHARE_VM_CONCOLIC_EXP_ARRAYINITEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_ARRAYINITEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/SymbolicOp.hpp"
#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

#include <stdio.h>
#include <vector>

class ArrayInitExpression : public Expression {
  static const int EXP_NAME_LENGTH = 16;

private:
  char _arr_str[EXP_NAME_LENGTH];
  exp_list_t _arr_exps;

public:
  ArrayInitExpression(sym_rid_t array_id, arrayOop array);
  ~ArrayInitExpression();

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_EXP_ARRAYINITEXPRESSION_HPP