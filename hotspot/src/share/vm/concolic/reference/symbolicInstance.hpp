#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINSTANCE_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINSTANCE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/reference/symbolicReference.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymInstance : public SymRef {
public:
  SymInstance(sym_rid_t sym_rid) : SymRef(sym_rid) {}
  ~SymInstance() {}

  virtual Expression *get(int field_offset) { return NULL; }
  virtual Expression *create_ref_exp(oop obj) {
    return new InstanceSymbolExp(obj);
  }
  virtual Expression *get_or_create_ref_exp(oop obj) {
    Expression *exp = get_ref_exp();
    if (exp == NULL) {
      exp = create_ref_exp(obj);
      set_ref_exp(exp);
    }
    return exp;
  }
  virtual Expression *get_ref_exp() = 0;

  virtual void set_ref_exp(Expression *) { ShouldNotCallThis(); }

  virtual void init_sym_exp(int field_offset, Expression *exp) {
    ShouldNotCallThis();
  }
  virtual void set_sym_exp(int field_offset, Expression *exp) = 0;

  virtual void print() = 0;
  virtual bool need_recursive() { return true; }

  inline void init_sym_exp(int field_offset, BasicType type) {
    return init_sym_exp(field_offset, new FieldSymbolExp(this->get_sym_rid(),
                                                         field_offset, type));
  }

  inline void init_sym_exp(int field_offset, BasicType type, std::string name) {
    return init_sym_exp(field_offset, new FieldSymbolExp(this->get_sym_rid(),
                                                         name, type));
  }

  static Expression* get_exp_of(oop obj);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINSTANCE_HPP