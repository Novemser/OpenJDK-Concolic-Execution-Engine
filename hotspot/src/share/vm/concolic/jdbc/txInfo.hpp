#ifndef SHARE_VM_CONCOLIC_JDBC_TXINFO_HPP
#define SHARE_VM_CONCOLIC_JDBC_TXINFO_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include <sys/types.h>
#include <vector>
#include "concolic/jdbc/reference/symbolicStatement.hpp"

class TxInfo {
private:
  bool _committed;
  tx_id_t _tx_id;
  std::vector<SymStmt *> _stmts;

public:
  TxInfo(tx_id_t tx_id) : _tx_id(tx_id), _committed(false) {}

  inline void commit() { _committed = true; }

  inline bool is_committed() { return _committed; }

  inline void record_stmt(SymStmt *stmt) {
    stmt->set_tx_id(_tx_id);
    _stmts.push_back(stmt);
  }

  std::vector<SymStmt *> &get_stmts() {
    return _stmts;
  }

public:
  void print();
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif //SHARE_VM_CONCOLIC_JDBC_TXINFO_HPP
