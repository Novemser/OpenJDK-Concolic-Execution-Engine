#ifndef SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICINSTANCE_HPP
#define SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICINSTANCE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymInstance {
protected:
  sym_oid_t _sym_oid;

public:
  SymInstance(sym_oid_t sym_oid) : _sym_oid(sym_oid) {}
  virtual ~SymInstance() {}

  inline sym_oid_t get_sym_oid() { return _sym_oid; }

  virtual void print() {}
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICINSTANCE_HPP