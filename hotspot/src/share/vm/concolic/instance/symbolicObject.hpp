#ifndef SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/instance/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymObj : public SymInstance{
  typedef std::map<int, Expression *> ExpStore;

private:
  ExpStore _exps;

public:
  SymObj(sym_oid_t sym_oid);
  ~SymObj();

  Expression *get(int field_index);

  void init_sym_exp(int field_index);
  void init_sym_exp(int field_index, Expression *exp);
  void set_sym_exp(int field_index, Expression *exp);

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICOBJECT_HPP