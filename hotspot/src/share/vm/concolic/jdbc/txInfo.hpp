#ifndef SHARE_VM_CONCOLIC_JDBC_TXSTATUS_HPP
#define SHARE_VM_CONCOLIC_JDBC_TXSTATUS_HPP
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include <sys/types.h>
#include <vector>
#include "concolic/jdbc/reference/symbolicStatement.hpp"

class TxInfo {
private:
  bool _committed;
  ulong _tx_id;
  std::vector<SymStmt *> _stmts;

public:
  TxInfo(ulong tx_id) : _tx_id(tx_id), _committed(false) {}

  inline void commit() { _committed = true; }

  inline bool is_committed() { return _committed; }

  inline void record_stmt(SymStmt *stmt) { _stmts.push_back(stmt); }

public:
  void print();
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif //SHARE_VM_CONCOLIC_JDBC_TXSTATUS_HPP
