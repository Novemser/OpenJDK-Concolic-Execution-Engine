#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICBIGDECIMAL_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICBIGDECIMAL_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymBigDecimal : public SymInstance {
public:
  static const char *TYPE_NAME;
  static std::set<std::string> handle_method_names;
  static std::set<std::string> init_handle_method_names();
  static std::map<std::string, bool> skip_method_names;
  static std::map<std::string, bool> init_skip_method_names();
  static bool need_recording;


private:
  Expression *_exp;

public:
  SymBigDecimal(sym_rid_t sym_rid);
  ~SymBigDecimal();

  inline static bool target(const std::string &class_name) {
    return class_name == TYPE_NAME;
  }

  Expression *get_ref_exp() { return _exp; };
  void set_ref_exp(Expression *exp) {
    Expression::gc(_exp);
    _exp = exp;
    _exp->inc_ref();
  };

  /**
   * TODO: try calling "toString" of bigdecimal
   * this is an ugly workaround...
   */
  inline Expression * create_ref_exp(oop obj) { return get_con_exp(obj); }

  bool need_recursive() { return false; }
  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static Expression *get_exp_of(oop obj);
  static Expression *get_con_exp(oop obj);
  static void init_register_class(MethodSymbolizer *m_symbolizer);

private:
  static method_set_t init_symbolized_methods();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICBIGDECIMAL_HPP