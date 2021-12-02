#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymStmt;
class StatementSymbolExp : public SymbolExpression {
public:
  StatementSymbolExp(SymStmt* sym_stmt);
};

class SymStmt : public SymInstance {
public:
  inline static bool target(const std::string &class_name) {
    return target_class_names.find(class_name) != target_class_names.end();
  }

private:
  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> skip_method_names;
  static std::set<std::string> init_skip_method_names();
  static std::map<std::string, BasicType> support_set_methods;
  static std::map<std::string, BasicType> init_support_set_methods();
  static Expression *get_param_exp(MethodSymbolizerHandle &handle, BasicType method, const std::string &callee_name);

private:
  std::string _sql_template;
  exp_map_t _param_exps;
  Expression* _row_count_exp;

public:
  SymStmt(sym_rid_t sym_rid);

  ~SymStmt();

public:
  inline void swap_sql_template(std::string &sql_template) {
    _sql_template.swap(sql_template);
  }

  inline void set_sql_template(const char *sql_template) {
    _sql_template = std::string(sql_template);
  }

  void set_param(int index, Expression *exp);

  void set_row_count_exp(Expression* row_count_exp);

public:
  bool need_recursive() { return false; }

  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);

  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP