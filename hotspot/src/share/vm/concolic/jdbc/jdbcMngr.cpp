#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include <concolic/fieldTraverser.hpp>
#include "concolic/concolicMngr.hpp"
#include "concolic/jdbc/jdbcMngr.hpp"
#include "concolic/utils.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

#include <string>

JdbcMngr::~JdbcMngr() {
//  ulong size = _txs.size();
//  for (ulong i = 0; i < size; ++i) {
//    delete _txs[i];
//  }
//  _txs.clear();
//  size_t size = _sym_stmt_list.size();
//  for (size_t stmt_index = 0; stmt_index < size; stmt_index++) {
//    delete _sym_stmt_list[stmt_index].first;
//  }
//  _sym_stmt_list.clear();
}

void JdbcMngr::set_auto_commit(jboolean auto_commit, jlong conn_id) {
  tty->print_cr("[Txn] set_auto_commit: %us, conn_id:%ld", auto_commit, conn_id);
//  std::map<jlong, TxInfo *>::iterator iter = _conn_ongoing_tx.find(conn_id);
  SymSetAutoCommit* autoCommit = new SymSetAutoCommit(auto_commit);
  autoCommit->set_pc(ConcolicMngr::ctx->get_path_condition());
  _sym_stmt_list.push_back(std::make_pair(autoCommit, conn_id));

//  if (!auto_commit) {
//    tx_id_t tx_id = _txs.size();
//    TxInfo *tx = new TxInfo(tx_id);
//    _txs.push_back(tx);
//
//    guarantee(iter == _conn_ongoing_tx.end(), "should be");
//    _conn_ongoing_tx[conn_id] = tx;
//  } else {
//    if (iter != _conn_ongoing_tx.end()) {
//      iter->second->commit();
//      _conn_ongoing_tx.erase(iter);
//    }
//  }
}

void JdbcMngr::commit(jlong conn_id) {
  tty->print_cr("[Txn] explicit commit:%ld", conn_id);
  SymCommit* commit = new SymCommit();
  commit->set_pc(ConcolicMngr::ctx->get_path_condition());
  _sym_stmt_list.push_back(std::make_pair(commit, conn_id));
}

void JdbcMngr::print() {
  // txns
//  tty->print("txns:");
//  ulong size = _txs.size();
//  for (ulong i = 0; i < size; ++i) {
//    _txs[i]->print();
//  }
  tty->print_cr("[Print] Sql list size:%lu", _sym_stmt_list.size());
  for (ulong i = 0; i < _sym_stmt_list.size(); i++) {
    std::pair<SymStmt *, jlong> &pair = _sym_stmt_list[i];
    tty->print("[Connection %lu] ", pair.second);
    pair.first->print();
  }
//  for (ulong i = 0; i < size; ++i) {
//    _txs[i]->print();
//  }
  // persistent obj
//  tty->print_cr("persistent objects:");
//  for (RidToStringIt pair = persistentObjStackTrace.begin(); pair != persistentObjStackTrace.end(); pair++) {
//    tty->print_cr("%ld: %s", pair->first, pair->second.c_str());
//  }
//
//  // checker
//  ulong unrecorded_select_count = 0;
//  ulong unrecorded_nonselect_count = 0;
//  ulong num_txs = _txs.size();
//  for (ulong i = 0; i < num_txs; ++i) {
//    TxInfo *tx = _txs[i];
//    std::vector<SymStmt*> stmts = tx->get_stmts();
//    ulong num_stmts = stmts.size();
//    tty->print_cr("|txn: %-8lu =====================================", i);
//    for (ulong j = 0; j < num_stmts; j++) {
//      if (stmts[j]->get_obj_rid() == 0) {
//        tty->print_cr("|%-8lu|  ????  |%-128s...|", stmts[j]->get_sym_rid(), stmts[j]->get_sql_template().c_str());
//        if (stmts[j]->get_sql_template().find("select") == 0) {
//          unrecorded_select_count++;
//        } else {
//          unrecorded_nonselect_count++;
//        }
//      } else {
//        tty->print_cr("|%-8lu|%-8lu|%-32s...|", stmts[j]->get_sym_rid(), stmts[j]->get_obj_rid(), stmts[j]->get_sql_template().c_str());
//      }
//    }
//    tty->print_cr("|===================================================");
//  }
//  tty->print_cr("total number of stmts that doesn't have a corresponding object: %lu",
//                unrecorded_select_count + unrecorded_nonselect_count);
//  tty->print_cr("among them %lu are select, %lu are non-select", unrecorded_select_count, unrecorded_nonselect_count);
//  tty->print_cr("note that if non-select is not 0, then you may need to check the process of collections (I didn't deal with them)!\n");

}

void JdbcMngr::record_stmt(SymStmt *stmt, jlong conn_id) {
//  tty->print_cr("[Recorder] Try recording stmt, printing");
//  stmt->print();
  _sym_stmt_list.push_back(std::make_pair(stmt, conn_id));

//  std::map<jlong, TxInfo *>::iterator iter = _conn_ongoing_tx.find(conn_id);
//  if (iter != _conn_ongoing_tx.end()) {
//    // There's no ongoing transaction in conn_id connection
//    guarantee(iter->second, "should be");
//    iter->second->record_stmt(stmt);
//    tty->print_cr("Recorded stmt into txn %lu", iter->first);
//  }
}

void JdbcMngr::record_stmt_obj_pair(oop stmt_or_proxy, oop obj) {
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
    guarantee(h, "should be");
    // offset = 32
    stmt = OopUtils::obj_field_by_name(h, "delegate", "Ljava/lang/Object;");
    guarantee(stmt, "should be");
  }

  if (stmt->is_symbolic() && obj->is_symbolic()) {
    guarantee(stmt_to_obj.find(stmt->get_sym_rid()) == stmt_to_obj.end(), "should be");
    stmt_to_obj[stmt->get_sym_rid()] = obj->get_sym_rid();
    SymStmt *sym_stmt = (SymStmt*)ConcolicMngr::ctx->get_sym_inst(stmt);
    sym_stmt->set_obj_rid(obj->get_sym_rid());
  }
}

void JdbcMngr::record_persistent_obj(oop obj) {
  if (obj == NULL) {
//     tty->print_cr("--- NULL record");
    return;
  }
  if (obj->is_symbolic()) {
    // tty->print_cr("--- record: ");
    // obj->print();
    persistentObjStackTrace[obj->get_sym_rid()] = ConcolicMngr::ctx->get_code_pos_for_first("broadleaf");
  } else {
//     tty->print_cr("--- not record: ");
  }
}

const std::vector<std::pair<SymStmt *, jlong> > &JdbcMngr::get_sym_stmt_list() const {
  return _sym_stmt_list;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC