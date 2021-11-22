#ifndef SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "concolic/defs.hpp"
#include "oops/oop.inline.hpp"

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

#endif

#endif // SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP