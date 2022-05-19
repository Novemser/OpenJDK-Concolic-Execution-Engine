#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPRIMITIVE_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPRIMITIVE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/utils.hpp"
#include <string>

template <class T> class SymPrimitive : public SymInstance {
public:
  static const char *ARRAY_TYPE_NAME;
  static const char *TYPE_NAME;

private:
  static sym_rid_t sym_count;
  Expression *_exp;

public:
  SymPrimitive(sym_rid_t sym_rid);
  ~SymPrimitive();

  Expression *get(int field_offset);
  Expression *get_ref_exp() { return _exp; };
  void set_ref_exp(Expression *exp) {
    Expression::gc(_exp);
    _exp = exp;
    _exp->inc_ref();
  };

  void init_sym_exp(int field_offset, Expression *exp);
  void set_sym_exp(int field_offset, Expression *exp);
  static void set_symbolic_field(oop obj, Symbol* fld_name, Symbol* fld_tp_sig, MethodSymbolizerHandle &handle);
  static bool match_callee_and_do_sym(MethodSymbolizerHandle &handle, const char* sig);
public:
  void print();

public:
  static Expression *get_exp_of(oop obj);
  static Expression *get_exp_of_helper(oop obj);
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
};

bool primitive_invoke_method_helper(MethodSymbolizerHandle &handle,BasicType type);
Expression *primitive_finish_method_helper(MethodSymbolizerHandle &handle,BasicType type);
BasicType primitive_target(const std::string &class_name);

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPRIMITIVE_HPP
