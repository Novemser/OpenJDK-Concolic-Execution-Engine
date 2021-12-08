#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/jdbcMngr.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

JdbcMngr::~JdbcMngr() {
  ulong size = _txs.size();
  for (ulong i = 0; i < size; ++i) {
    delete _txs[i];
  }
  _txs.clear();
}

void JdbcMngr::set_auto_commit(jboolean auto_commit, jlong conn_id) {
  tty->print_cr("set_auto_commit: %us", auto_commit);
  if (!auto_commit) {
    int tx_id = _txs.size();
    TxInfo *tx = new TxInfo(tx_id);
    _txs.push_back(tx);
    if (_conn_ongoing_tx.find(conn_id) == _conn_ongoing_tx.end()) {
      _conn_ongoing_tx[conn_id] = tx;
    } else {
      // we assume one connection only have one ongoing transaction
      ShouldNotReachHere();
    }
  } else {
    if (_conn_ongoing_tx.find(conn_id) != _conn_ongoing_tx.end()) {
      _conn_ongoing_tx[conn_id]->commit();
      _conn_ongoing_tx.erase(conn_id);
    }
  }
}

void JdbcMngr::print() {
  ulong size = _txs.size();
  for (ulong i = 0; i < size; ++i) {
    _txs[i]->print();
  }
}

void JdbcMngr::record_stmt(SymStmt *stmt, jlong conn_id) {
  // There's no ongoing transaction in conn_id connection
  if (_conn_ongoing_tx.find(conn_id) == _conn_ongoing_tx.end() ) {
    return;
  }
  _conn_ongoing_tx[conn_id]->record_stmt(stmt);
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC