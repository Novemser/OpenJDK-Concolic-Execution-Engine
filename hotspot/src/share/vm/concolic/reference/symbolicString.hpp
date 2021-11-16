#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"
#include <string>

class SymString : public SymInstance {
private:
  Expression *_exp;

public:
  SymString(sym_rid_t sym_rid);
  ~SymString();

  Expression *get(int field_offset);

  void init_sym_exp(int field_offset);
  void init_sym_exp(int field_offset, Expression *exp);
  void set_sym_exp(int field_offset, Expression *exp);

public:
  void print();

public:
  static bool invoke_method(MethodSymbolizer::Handle& handle);
  static void finish_method(MethodSymbolizer::Handle& handle);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP