#ifndef SHARE_VM_CONCOLIC_JDBC_TXSTATUS_HPP
#define SHARE_VM_CONCOLIC_JDBC_TXSTATUS_HPP
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include <sys/types.h>

class TxInfo {
private:
  bool _committed;
  ulong _tx_id;
public:
  TxInfo(ulong tx_id) : _tx_id(tx_id), _committed(false) {}

  inline void commit() { _committed = true; }

  inline bool is_committed() { return _committed; }

public:
  void print();
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif //SHARE_VM_CONCOLIC_JDBC_TXSTATUS_HPP
