#ifndef SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_SYMBOLEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"

#include <string>

/**
 * The format of output of Symbol_Expression:
 *     {Y/M/N/V}_{Z/C/B/S/I/F/J/D/[xxx/Lxxx}_{self-defined part}
 *     1. Whether this exp is constant: "Y"->const, "M"->non-const, "N" -> NULL, "V" -> Void
 *     2. The type of this exp
 *     3. self-defined part for each extended expression
 */

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
  static char temp_buf[BUF_SIZE];

  SymbolExpression() {}
  SymbolExpression(const char *buf, int length) { set(buf, length);}

protected:
  std::string _str;
  std::string _type;

  /**
   * The following calls must be called in order!
   */
  static void set_head(stringStream& ss, char main_type, BasicType class_type, oop obj = NULL);
  static void set_head(stringStream& ss, char main_type, BasicType class_type, const char* klass_name);
  void finalize(int length);

public:
  void print();
  void set(const char *buf, int length);

  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

  inline void set_type(std::string new_type) { _type = new_type; }
};

class InstanceSymbolExp : public SymbolExpression {
public:
  InstanceSymbolExp(oop obj);
};

class PlaceHolderSymbolExp : public SymbolExpression {
public:
  PlaceHolderSymbolExp(oop obj);
};

class FieldSymbolExp : public SymbolExpression {
public:
  FieldSymbolExp(sym_rid_t sym_rid, int field_index, BasicType type);
  FieldSymbolExp(sym_rid_t sym_rid, std::string name, BasicType type);
};

class ConSymbolExp : public SymbolExpression {
public:
  ConSymbolExp(const char* str, BasicType type);
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);
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