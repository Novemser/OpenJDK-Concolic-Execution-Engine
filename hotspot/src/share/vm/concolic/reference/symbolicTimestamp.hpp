#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICTIMESTAMP_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICTIMESTAMP_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymTimestamp : public SymInstance {
public:
  static const char *TYPE_NAME;
  static method_set_t symbolized_methods;
private:
  Expression *_exp;
  Expression *_exp_converted;
  std::map<int, Expression*> _internal_fields;
  int _fastTimeFldOffset;

public:
  SymTimestamp(sym_rid_t sym_rid);
  SymTimestamp(sym_rid_t sym_rid, oop obj);
  ~SymTimestamp();

  inline static bool target(const std::string &class_name) {
    return class_name == TYPE_NAME;
  }

  Expression *get_ref_exp();

  void set_ref_exp(Expression *exp) {
    Expression::gc(_exp);
    _exp = exp;
    _exp->inc_ref();
  };

  bool need_recursive() { return true; }
  void print();

  virtual void init_sym_exp(int field_offset, Expression *exp);

  virtual Expression *get(int field_offset);

  virtual void set_sym_exp(int field_offset, Expression *exp);

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static Expression *get_exp_of(oop obj);
  static void init_register_class(MethodSymbolizer *m_symbolizer);

private:
  static method_set_t init_symbolized_methods();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICTIMESTAMP_HPP