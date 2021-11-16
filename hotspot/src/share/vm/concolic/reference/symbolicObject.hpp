#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/reference/symbolicReference.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymObj : public SymRef{
  typedef std::map<int, Expression *> ExpStore;

private:
  ExpStore _exps;

public:
  SymObj(sym_oid_t sym_oid);
  ~SymObj();

  Expression *get(int field_offset);

  void init_sym_exp(int field_offset);
  void init_sym_exp(int field_offset, Expression *exp);
  void set_sym_exp(int field_offset, Expression *exp);

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICOBJECT_HPP