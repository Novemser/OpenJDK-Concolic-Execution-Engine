#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymStmt : public SymInstance {
public:
  static const char *TYPE_NAME;

private:
  std::string _sql_template;

public:
  SymStmt(sym_rid_t sym_rid);
  ~SymStmt();

public:
  inline void swap_sql_template(std::string &sql_template) {
    _sql_template.swap(sql_template);
  }

public:
  Expression *get(int field_offset) {
    ShouldNotCallThis();
    return NULL;
  }
  Expression *get_ref_exp() {
    ShouldNotCallThis();
    return NULL;
  }
  void set_ref_exp(Expression *exp) { ShouldNotCallThis(); }

  void init_sym_exp(int field_offset, Expression *exp) { ShouldNotCallThis(); }
  void set_sym_exp(int field_offset, Expression *exp) { ShouldNotCallThis(); }

  bool need_recursive() { return false; }

  void print();
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP