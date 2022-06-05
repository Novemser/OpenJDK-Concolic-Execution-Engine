#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymResSet;
class SymStmt;
class ResultSetSymbolExp : public SymbolExpression {
public:
  ResultSetSymbolExp(SymResSet *sym_res_set, bool is_size = false);
  ResultSetSymbolExp(SymStmt *sym_stmt);
  ResultSetSymbolExp(SymResSet *sym_res_set, const char *col_name, BasicType type, oop obj);
  ResultSetSymbolExp(SymResSet *sym_res_set, int col_i, BasicType type, oop obj);
};
class SymResSet : public SymInstance {
  friend class ResultSetSymbolExp;

public:
  inline static bool target(const std::string &class_name) {
    return target_class_names.find(class_name) != target_class_names.end();
  }
protected:
  void get_ret_val_name(int col_i, std::string& input);

private:
  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> handle_method_names;
  static std::set<std::string> init_handle_method_names();
  static std::set<std::string> skip_method_names;
  static std::set<std::string> init_skip_method_names();

private:
  sym_rid_t _sql_id;
  int _row_id;
  SymStmt* _sym_stmt;

  Expression* _ref_exp;
  jint _last_got_index; // used to store the index of result attribute accessed by getXXX(String)

public:
  SymResSet(sym_rid_t sym_rid);
  ~SymResSet();

  /**
   * Handle these redandent code
   */
  Expression *get_ref_exp() { return _ref_exp; };

  void set_ref_exp(Expression *exp) {
    Expression::gc(_ref_exp);
    _ref_exp = exp;
    _ref_exp->inc_ref();
  };
public:
  void set_sym_stmt(SymStmt* sym_stmt);

  inline int get_row_id() { return _row_id; }

  inline void next() { ++_row_id; }

  SymStmt *get_sym_stmt();

  virtual void set_sym_exp(int field_offset, Expression *exp);

public:
  bool need_recursive() { return false; }

  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void init_register_class(MethodSymbolizer *m_symbolizer);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP