#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include <concolic/fieldTraverser.hpp>
#include "concolic/concolicMngr.hpp"
#include "concolic/jdbc/jdbcMngr.hpp"
#include "concolic/utils.hpp"
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
  tty->print_cr("[Txn] set_auto_commit: %us, conn_id:%ld", auto_commit, conn_id);
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
  // stmt and obj
  for (StmtToObjIt pair = stmt_to_obj.begin(); pair != stmt_to_obj.end(); pair++) {
    sym_rid_t stmt_rid = pair->first;
    sym_rid_t obj_rid = pair->second;
    SymStmt *stmt = (SymStmt*)ConcolicMngr::ctx->get_sym_inst(stmt_rid);
    tty->print("StmtObjRidPair: stmt(%ld -> %ld)-obj(%ld)", stmt_rid, stmt->get_obj_rid(), obj_rid);
    guarantee(stmt->get_obj_rid() == obj_rid, "should be");
    tty->print("\n* %s\n", stmt->get_sql_template().c_str());
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

void __attribute__((optimize("O0"))) JdbcMngr::record_stmt_obj_pair(oop stmt_or_proxy, oop obj) {
  ResourceMark rm;
  oop stmt = stmt_or_proxy;
  // try to get stmt from proxy
  const char *stmt_or_proxy_klass_name = stmt_or_proxy->klass()->name()->as_C_string();
  if (strlen(stmt_or_proxy_klass_name) < 20) {
    tty->print_cr("strange: %s", stmt_or_proxy_klass_name);
  }
  if (strncmp(stmt_or_proxy_klass_name, "com/sun/proxy/$Proxy", 20) == 0) {
    // offset = 24
    oop h = OopUtils::obj_field_by_name(stmt, "h", "Ljava/lang/reflect/InvocationHandler;");
    // offset = 32
    if (h == NULL) {
      tty->print_cr("h is NULL");
      return;
    }
    stmt = OopUtils::obj_field_by_name(h, "delegate", "Ljava/lang/Object;");
    if (stmt == NULL) {
      tty->print_cr("stmt is NULL");
      return;
    }
  }

  if (stmt->is_symbolic()) {
    tty->print_cr("stmt is symbolic");
  } else {
    tty->print_cr("stmt is not symbolic");
  }
  if (obj->is_symbolic()) {
    ConcolicMngr::ctx->get_sym_inst(obj)->print();
  }
  if (stmt->is_symbolic() && obj->is_symbolic()) {
    guarantee(stmt_to_obj.find(stmt->get_sym_rid()) == stmt_to_obj.end(), "should be");
    stmt_to_obj[stmt->get_sym_rid()] = obj->get_sym_rid();
    SymStmt *sym_stmt = (SymStmt*)ConcolicMngr::ctx->get_sym_inst(stmt);
    sym_stmt->set_obj_rid(obj->get_sym_rid());
    tty->print_cr("StmtObjRidPair: stmt(%ld -> %ld)-obj(%ld)", sym_stmt->get_sym_rid(), sym_stmt->get_obj_rid(), obj->get_sym_rid());
  }
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC