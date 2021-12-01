#ifndef SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"

#include <string>

enum BuiltInSym {
  Sym_NULL = 0,
  Sym_VOID,
  number_of_builtin_syms
};

class SymbolExpression : public Expression {
  static SymbolExpression* shared_exp[number_of_builtin_syms];
public:
  static void init_single_instances();
  static SymbolExpression *get(BuiltInSym type) { return shared_exp[type];}

public:
  static const int BUF_SIZE = 1024;
  static char str_buf[BUF_SIZE];

  SymbolExpression() {}
  SymbolExpression(const char *buf, int length) { set(buf, length);}

protected:
  std::string _str;

  void set(const char *buf, int length);

public:
  void print();
};

class InstanceSymbolExp : public SymbolExpression {
public:
  InstanceSymbolExp(oop obj);
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


#endif

#endif // SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP