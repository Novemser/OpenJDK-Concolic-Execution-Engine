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

void JdbcMngr::set_auto_commit(jboolean auto_commit) {
  tty->print_cr("set_auto_commit: %us", auto_commit);
  if (!auto_commit) {
    guarantee(!has_ongoing_tx(), "should be");
    TxInfo *tx = new TxInfo(_txs.size());
    _txs.push_back(tx);
  } else {
    guarantee(has_ongoing_tx(), "should be");
    TxInfo *last_tx = _txs.back();
    last_tx->commit();
  }
}

void JdbcMngr::print() {
  ulong size = _txs.size();
  for (ulong i = 0; i < size; ++i) {
    _txs[i]->print();
  }
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC