#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"

class SymString : public SymInstance {
public:
  static const char *TYPE_NAME;
  static method_set_t symbolized_methods;

private:
  static sym_rid_t sym_string_count;

  Expression *_ref_exp;
  Expression *_exp;

public:
  SymString(sym_rid_t sym_rid);
  ~SymString();

  Expression *get(int field_offset);
  Expression *get_ref_exp() { return _ref_exp; };
  void set_ref_exp(Expression *exp) {
    if (_ref_exp && _ref_exp->dec_ref()) {
      delete _ref_exp;
    }
    _ref_exp = exp;
    _ref_exp->inc_ref();
  };

  void init_sym_exp(int field_offset, Expression *exp);
  void set_sym_exp(int field_offset, Expression *exp);

  bool need_recursive() { return false; }
  void print();

public:
  static bool invoke_method(MethodSymbolizerHandle &handle);
  static void finish_method(MethodSymbolizerHandle &handle);

  static int prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                           intptr_t *locals, int locals_offset, bool &need_symbolize);
  static Expression *get_exp_of(oop obj);

private:
  static method_set_t init_symbolized_methods();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP