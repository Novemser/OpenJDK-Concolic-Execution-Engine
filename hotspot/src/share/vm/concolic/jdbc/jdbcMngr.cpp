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
  tty->print_cr("set_auto_commit: %us, conn_id:%ld", auto_commit, conn_id);
  std::map<jlong, TxInfo *>::iterator iter = _conn_ongoing_tx.find(conn_id);

  if (!auto_commit) {
    tx_id_t tx_id = _txs.size();
    TxInfo *tx = new TxInfo(tx_id);
    _txs.push_back(tx);

    guarantee(iter == _conn_ongoing_tx.end(), "should be");
    _conn_ongoing_tx[conn_id] = tx;
  } else {
    if (iter != _conn_ongoing_tx.end()) {
      iter->second->commit();
      _conn_ongoing_tx.erase(iter);
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
  std::map<jlong, TxInfo *>::iterator iter = _conn_ongoing_tx.find(conn_id);
  if (iter != _conn_ongoing_tx.end()) {
    // There's no ongoing transaction in conn_id connection
    guarantee(iter->second, "should be");
    iter->second->record_stmt(stmt);
  }
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC