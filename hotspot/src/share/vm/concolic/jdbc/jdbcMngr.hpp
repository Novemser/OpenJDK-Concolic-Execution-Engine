#ifndef SHARE_VM_CONCOLIC_JDBC_JDBCMNGR_HPP
#define SHARE_VM_CONCOLIC_JDBC_JDBCMNGR_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "jni.h"
#include "concolic/jdbc/txInfo.hpp"

#include <vector>

class JdbcMngr {
private:
  std::vector<TxInfo *> _txs;
public:
  JdbcMngr() {}
  ~JdbcMngr();

  void set_auto_commit(jboolean auto_commit);

public:
  bool has_ongoing_tx() {
    return _txs.size() != 0 && !_txs.back()->is_committed();
  }

public:
  void print();
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_JDBCMNGR_HPP