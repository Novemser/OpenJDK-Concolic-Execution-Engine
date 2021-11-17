#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINTEGER_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINTEGER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/utils.hpp"
#include <string>

class SymInteger : public SymInstance {
public:
  static const char *ARRAY_TYPE_NAME;
  static const char *TYPE_NAME;

private:
  static sym_rid_t sym_int_count;

  // Expression *_ref_exp;
  Expression *_exp;

public:
  SymInteger(sym_rid_t sym_rid);
  ~SymInteger();

  Expression *get(int field_offset);
  Expression *get_ref_exp() { return _exp; };
  void set_ref_exp(Expression *exp) {
    if (_exp && _exp->dec_ref()) {
      delete _exp;
    }
    _exp = exp;
    _exp->inc_ref();
  };

  void init_sym_exp(int field_offset);
  void init_sym_exp(int field_offset, Expression *exp);
  void set_sym_exp(int field_offset, Expression *exp);

public:
  void print();

public:
  static Expression *get_exp_of(oop obj);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICINTEGER_HPP