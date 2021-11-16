#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICREFERENCE_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICREFERENCE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymRef {
protected:
  sym_oid_t _sym_oid;

public:
  SymRef(sym_oid_t sym_oid) : _sym_oid(sym_oid) {}
  virtual ~SymRef() {}

  inline sym_oid_t get_sym_oid() { return _sym_oid; }

  virtual void print() {}
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICREFERENCE_HPP