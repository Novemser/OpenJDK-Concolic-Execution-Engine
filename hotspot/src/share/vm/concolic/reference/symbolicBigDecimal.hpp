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
#ifdef ENABLE_WEBRIDGE
  static std::string DECIMAL_WRAPPER;
#endif

private:
#ifdef ENABLE_WEBRIDGE
  std::map<int, Expression*> _internal_fields;
  int int_compact_offset(oop decimal);
  int scale_offset(oop decimal);
  void symbolize_bigDecimal(oop decimalOOp, Expression* parentExp);
  int _scale_offset;
  int _int_compact_offset;
public:
  virtual void init_sym_exp(int field_offset, Expression *exp);
  void set_bigDecimal_symbolic(oop decimalOOp, std::string name);

  virtual Expression *get(int field_offset);

private:
#endif
  Expression *_exp;
public:
  SymBigDecimal(sym_rid_t sym_rid, oop obj);
  ~SymBigDecimal();

  inline static bool target(const std::string &class_name) {
    return class_name == TYPE_NAME;
  }

  Expression *get_ref_exp();
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

  virtual void set_sym_exp(int field_offset, Expression *exp);

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static Expression *get_exp_of(oop obj);
  static Expression *get_con_exp(oop obj);
  static void init_register_class(MethodSymbolizer *m_symbolizer);
#ifdef ENABLE_WEBRIDGE
  static void method_exit_callback(MethodSymbolizerHandle &handle);
#endif
private:
  static method_set_t init_symbolized_methods();
};

class BigDecimalExpression : public Expression {
private:
  Expression* _scale;
  Expression* _intCompact;
protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

public:
  BigDecimalExpression(Expression* _scale, Expression* _intCompact);
  ~BigDecimalExpression();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICBIGDECIMAL_HPP