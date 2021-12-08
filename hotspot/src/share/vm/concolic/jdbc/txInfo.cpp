#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/txInfo.hpp"
#include "utilities/ostream.hpp"


void TxInfo::print() {
  tty->print_cr("TxInfo (%lu): %s", _tx_id, is_committed() ? "committed" : "not committed");
  tty->print_cr("-SymStmts in this Tx:");
  for (std::vector<SymStmt *>::iterator it = _stmts.begin(); it != _stmts.end(); ++it) {
    (*it)->print();
  }
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC