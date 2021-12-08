#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/txInfo.hpp"
#include "utilities/ostream.hpp"


void TxInfo::print() {
  tty->print_cr("TxInfo (%lu): %s", _tx_id, is_committed() ? "committed" : "not committed");
  int size = _stmts.size();
  for (int i = 0; i < size; ++i) {
    _stmts[i]->print();
  }
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC