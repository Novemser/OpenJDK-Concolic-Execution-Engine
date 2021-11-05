#ifndef SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymbolicObject {
  typedef std::map<int, Expression *> SymExpStore;

private:
  sym_oid_t _sym_oid;
  SymExpStore _sym_exps;

public:
  SymbolicObject(sym_oid_t sym_oid);
  ~SymbolicObject();

  Expression *get(int field_index);

  void init_sym_exp(int field_index);
  void set_sym_exp(int field_index, Expression *sym_exp);

  inline sym_oid_t get_sym_oid() { return _sym_oid; }

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP