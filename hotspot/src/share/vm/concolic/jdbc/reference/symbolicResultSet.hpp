#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymResSet;
class ResultSetSymbolExp : public SymbolExpression {
public:
  ResultSetSymbolExp(SymResSet *sym_res_set, const char *col_name);
};
class SymResSet : public SymInstance {
  friend class ResultSetSymbolExp;

public:
  static const char *TYPE_NAME;
  static const char *BASE_TYPE_NAME;

private:
  sym_rid_t _sym_stmt_rid;
  int _sql_id;
  int _row_id;

public:
  SymResSet(sym_rid_t sym_rid);
  ~SymResSet();

public:
  inline void set_stmt_rid(sym_rid_t sym_stmt_rid) {
    _sym_stmt_rid = sym_stmt_rid;
    _sql_id = (int)sym_stmt_rid;
  }

  inline void next() { ++_row_id; }

public:
  bool need_recursive() { return false; }

  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP