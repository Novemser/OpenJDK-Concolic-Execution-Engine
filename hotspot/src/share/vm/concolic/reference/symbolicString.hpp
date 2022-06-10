#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymString : public SymInstance {
public:
  static const char *TYPE_NAME;
  static const char *ARRAY_TYPE_NAME;
  static method_set_t handle_method_names;
  static std::map<std::string, bool> skip_method_names;
  static bool need_recording;
  static std::map<std::string, oop> INTERN_MAP;
private:
  Expression *_ref_exp;
  Expression *_exp;

public:
  SymString(sym_rid_t sym_rid);
  ~SymString();

  Expression *get_ref_exp() { return _ref_exp; };
  void set_ref_exp(Expression *exp) {
    Expression::gc(_ref_exp);
    _ref_exp = exp;
    _ref_exp->inc_ref();
  };

  bool need_recursive() { return false; }
  void print();

  virtual void set_sym_exp(int field_offset, Expression *exp);

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void prepare_param(MethodSymbolizerHandle &handle);
  static int prepare_param_helper(MethodSymbolizerHandle &handle, BasicType type,
                                  int locals_offset);

  static Expression *get_exp_of(oop obj);

  static void init_register_class(MethodSymbolizer* m_symbolizer);

private:
  static method_set_t init_handle_method_names();
  static std::map<std::string, bool> init_skip_method_names();
  static bool get_condition_value(oop s1, oop s2, std::string comp);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP