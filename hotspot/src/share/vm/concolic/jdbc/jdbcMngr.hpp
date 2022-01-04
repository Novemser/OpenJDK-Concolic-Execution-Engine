#ifndef SHARE_VM_CONCOLIC_JDBC_JDBCMNGR_HPP
#define SHARE_VM_CONCOLIC_JDBC_JDBCMNGR_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "jni.h"
#include "concolic/defs.hpp"
#include "concolic/jdbc/txInfo.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include <vector>
#include <map>

class JdbcMngr {
private:
  std::vector<TxInfo *> _txs;
  std::map<jlong, TxInfo *> _conn_ongoing_tx;

public:
  JdbcMngr() {}

  ~JdbcMngr();

  void set_auto_commit(jboolean auto_commit, jlong conn_id);

public:
//  bool has_ongoing_tx(jlong conn_id) {
//    return _conn_ongoing_tx[conn_id] != NULL;
//  }

public:
  void print();

  void record_stmt(SymStmt *stmt, jlong conn_id);

private:
  typedef std::map<sym_rid_t, sym_rid_t>::iterator StmtToObjIt;
  std::map<sym_rid_t, sym_rid_t> stmt_to_obj;

public:
  void record_stmt_obj_pair(oop stmt, oop obj);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_JDBCMNGR_HPP