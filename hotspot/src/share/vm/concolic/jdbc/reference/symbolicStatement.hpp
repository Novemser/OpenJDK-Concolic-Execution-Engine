#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymStmt : public SymInstance {
public:
  static const char *TYPE_NAME;
  static const char *BASE_TYPE_NAME;

private:
  std::string _sql_template;
  exp_map_t _param_exps;

public:
  SymStmt(sym_rid_t sym_rid);
  ~SymStmt();

public:
  inline void swap_sql_template(std::string &sql_template) {
    _sql_template.swap(sql_template);
  }

  void set_param(int index, Expression *exp);

public:
  bool need_recursive() { return false; }
  void print();

public:
  static bool invoke_method(MethodSymbolizerHandle &handle);
  static void finish_method(MethodSymbolizerHandle &handle);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP