//
// Created by gansen on 4/21/22.
//

#include "webridgeMngr.hpp"

#ifdef ENABLE_WEBRIDGE

void webridgeMngr::analyse(ThreadContext *ctx) {
  JdbcMngr jdbc_mgr = ctx->get_jdbc_mngr();
  std::vector<std::pair<SymStmt *, jlong> > sym_stmt_list =
      jdbc_mgr.get_sym_stmt_list();
  tty->print_cr("[WeBridge] Received %ld SQL Statements", sym_stmt_list.size());
}

#endif