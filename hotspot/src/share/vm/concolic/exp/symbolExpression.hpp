#ifndef SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"

#include <string>

class SymbolExpression : public Expression {
public:
  static const int BUF_SIZE = 1024;
  static char str_buf[BUF_SIZE];

protected:
  std::string _str;

  void set(const char *buf, int length);

public:
  void print();
};

class InstanceSymbolExp : public SymbolExpression {
public:
  InstanceSymbolExp(sym_rid_t sym_rid, BasicType type);
};

class FieldSymbolExp : public SymbolExpression {
public:
  FieldSymbolExp(sym_rid_t sym_rid, int field_index, BasicType type);
};

class ArraySymbolExp : public SymbolExpression {
public:
  ArraySymbolExp(sym_rid_t sym_arr_oid, int arr_version, BasicType type);
};

class ArrayLengthExp : public SymbolExpression {
public:
  ArrayLengthExp(sym_rid_t sym_arr_oid, BasicType type);
};

class ElementSymbolExp : public SymbolExpression {
public:
  ElementSymbolExp(sym_rid_t sym_arr_oid, int arr_version, int load_count,
                   BasicType type);
};

class MethodReturnSymbolExp : public SymbolExpression {
  static sym_rid_t sym_method_count;

public:
  MethodReturnSymbolExp();
};

class StringSymbolExp : public SymbolExpression {
  static sym_rid_t sym_string_count;

public:
  StringSymbolExp();
};

#endif

#endif // SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP