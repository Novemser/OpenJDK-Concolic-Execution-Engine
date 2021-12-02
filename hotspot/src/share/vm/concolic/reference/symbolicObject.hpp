#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
class SymObj : public SymInstance {
private:
  exp_map_t _exps;
  Expression* _ref_exp;

public:
  SymObj(sym_rid_t sym_rid);
  ~SymObj();

  Expression *get(int field_offset);
  Expression *get_ref_exp() { return _ref_exp; };

  void set_ref_exp(Expression *exp) {
    Expression::gc(_ref_exp);
    _ref_exp = exp;
    _ref_exp->inc_ref();
  };

  void init_sym_exp(int field_offset, Expression *exp);
  void set_sym_exp(int field_offset, Expression *exp);

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICOBJECT_HPP