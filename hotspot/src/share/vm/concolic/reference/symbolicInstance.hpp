#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINSTANCE_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINSTANCE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/reference/symbolicReference.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymInstance : public SymRef {
public:
  SymInstance(sym_rid_t sym_rid) : SymRef(sym_rid) {}
  ~SymInstance() {}

  virtual Expression *get(int field_offset) = 0;
  virtual Expression *get_ref_exp() = 0;
  virtual void set_ref_exp(Expression *) = 0;

  virtual void init_sym_exp(int field_offset, Expression *exp) = 0;
  virtual void set_sym_exp(int field_offset, Expression *exp) = 0;

  inline void init_sym_exp(int field_offset, BasicType type) {
    return init_sym_exp(field_offset, new SymbolExpression(this->get_sym_rid(),
                                                           field_offset, type));
  }

public:
  virtual void print() = 0;
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINSTANCE_HPP