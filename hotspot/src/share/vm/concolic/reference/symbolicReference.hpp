#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICREFERENCE_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICREFERENCE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "oops/oop.inline.hpp"

#include <stdio.h>

class SymRef {
protected:
  sym_rid_t _sym_rid;

public:
  SymRef(sym_rid_t sym_rid) : _sym_rid(sym_rid) {}
  virtual ~SymRef() {}

  inline sym_rid_t get_sym_rid() { return _sym_rid; }

  virtual Expression* get_ref_exp() = 0;

  virtual void print() = 0;
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICREFERENCE_HPP