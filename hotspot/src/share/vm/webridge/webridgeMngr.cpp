//
// Created by gansen on 4/21/22.
//

#include <iostream>
#include <fstream>

#ifdef ENABLE_WEBRIDGE

#include "webridgeMngr.hpp"
#include "utils/jsonUtils.hpp"


#include "jni.h"
#include "runtime/interfaceSupport.hpp"
#include "jvm_misc.hpp"

std::string webridgeMngr::analyse(ThreadContext *ctx, Klass *weBridgeSPEntryKlass, JNIEnv *env) {
  if (!ctx) {
    tty->print_cr("[WeBridge] No associated thread context found, concolic execution might not enabled!");
    return "";
  }

  JdbcMngr jdbc_mgr = ctx->get_jdbc_mngr();
  const std::vector<std::pair<SymStmt *, jlong> > &sym_stmt_list =
      jdbc_mgr.get_sym_stmt_list();
  tty->print_cr("[WeBridge] Received %ld SQL Statements", sym_stmt_list.size());
  if (sym_stmt_list.empty()) {
    return "";
  }

//  JavaVM *jvm;
//  JavaValue result(T_VOID);
//  Thread *currentThread = Thread::current();
//  ResourceMark rm;
//  KlassHandle klass(currentThread, weBridgeSPEntryKlass);
//  assert(!klass.is_null(), "Invalid WeBridgeSPEntryKlass!");
  // TODO: replace the stub with WeBridge processing methods
  std::string argCppStr = jsonUtils::statementsToJsonString(sym_stmt_list);

  saveTemp(argCppStr);
  return argCppStr;
//  Handle arg;
//  JavaCalls::call_static(
//      &result, klass,
//      vmSymbols::_add_SQLTemplate(),
//      vmSymbols::_add_SQLTemplate_signature(),
//      arg, currentThread
//  );
}

static std::string tmp_file_location = "/tmp/webridgeMngr.tmp.sp.string";

void webridgeMngr::saveTemp(const std::string &str) {
  std::fstream out_file;
  out_file.open(tmp_file_location.c_str(), std::ios::out);
  if (!out_file) {
    tty->print_cr("[WeBridge] Failed to store temporary string!");
    assert(false, (std::string("Check") + tmp_file_location).c_str());
  }
  out_file << str;
  out_file.close();
}

#endif