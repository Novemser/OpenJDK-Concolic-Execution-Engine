#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymResSet : public SymInstance {
public:
  static const char *TYPE_NAME;
  static const char *BASE_TYPE_NAME;

private:
  sym_rid_t _sym_stmt_rid;
  int _row_id;

public:
  SymResSet(sym_rid_t sym_rid);
  ~SymResSet();

public:
  inline void set_stmt_rid(sym_rid_t sym_stmt_rid) {
    _sym_stmt_rid = sym_stmt_rid;
  }

  inline void next() { ++_row_id; }

public:
  bool need_recursive() { return false; }

  void print();

public:
  static bool invoke_method(MethodSymbolizerHandle &handle);
  static void finish_method(MethodSymbolizerHandle &handle);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP