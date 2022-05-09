//
// Created by gansen on 4/21/22.
//

#include <iostream>
#include <fstream>

#include "webridgeMngr.hpp"
#include "utils/jsonUtils.hpp"

#ifdef ENABLE_WEBRIDGE

#include "jni.h"
#include "runtime/interfaceSupport.hpp"
#include "jvm_misc.hpp"

void webridgeMngr::analyse(ThreadContext *ctx, Klass *weBridgeSPEntryKlass, JNIEnv *env) {
  if (!ctx) {
    tty->print_cr("[WeBridge] No associated thread context found, concolic execution might not enabled!");
    return;
  }

  JdbcMngr jdbc_mgr = ctx->get_jdbc_mngr();
  const std::vector<std::pair<SymStmt *, jlong> > &sym_stmt_list =
      jdbc_mgr.get_sym_stmt_list();
  tty->print_cr("[WeBridge] Received %ld SQL Statements", sym_stmt_list.size());
  if (sym_stmt_list.empty()) {
    return;
  }

  JavaVM *jvm;
  JavaValue result(T_VOID);
  Thread *currentThread = Thread::current();
  ResourceMark rm;
  KlassHandle klass(currentThread, weBridgeSPEntryKlass);
  assert(!klass.is_null(), "Invalid WeBridgeSPEntryKlass!");
  // TODO: replace the stub with WeBridge processing methods
  std::string argCppStr = jsonUtils::statementsToJsonString(sym_stmt_list);

  saveTemp(argCppStr);

  Handle arg;
  JavaCalls::call_static(
      &result, klass,
      vmSymbols::_add_SQLTemplate(),
      vmSymbols::_add_SQLTemplate_signature(),
      arg, currentThread
  );
//  jlong val = result.get_jlong();

//  JavaVMInitArgs vm_args;
//  JavaVMOption *options = new JavaVMOption[1];
//  options[0].optionString = const_cast<char*>(std::string("-Djava.class.path=.").c_str());
//  vm_args.version = JNI_VERSION_1_8;
//  vm_args.nOptions = 1;
//  vm_args.options = options;
//  vm_args.ignoreUnrecognized = false;
  jsize nVM = 0;
  jint rc = JNI_GetCreatedJavaVMs(&jvm, 1, &nVM);
//  jint rc = JNI_CreateJavaVM(&jvm,
//                             (void **)&env, &vm_args);
//  delete[] options;


  if (rc != JNI_OK || nVM != 1) {
    tty->print_cr("JNI not OK:%d, nVm:%d. Exiting WeBridge", rc, nVM);
    return;
  }

//  jvm->GetEnv((void **) &env, JNI_VERSION_1_8);
  if (env == NULL) {
    tty->print_cr("JNI env not initialized. Exiting WeBridge");
    return;
  }

  // 0. Calling version
  jint ver = env->GetVersion();
  tty->print_cr("JVM load successded: version %d.%d", (ver >> 16) & 0x0f, ver & 0x0f);
//  ResourceMark rm(currentThread);
//  ThreadToNativeFromVM ttn(JavaThread::thread_from_jni_environment(env));

  // 1. Calling MyTest
//  jclass cls2 = env->FindClass("MyTest");
//  if (cls2 == NULL) {
//    std::cerr << "Class not found" << std::endl;
//  } else {
//    std::cout << "Class MyTest found" << std::endl;
//    jmethodID mid = env->GetStaticMethodID(cls2, "mymain", "()V");
//    if (mid != NULL) {
//      env->CallStaticVoidMethod(cls2, mid);
//      std::cout << std::endl;
//    } else {
//      std::cerr << "No method found" << std::endl;
//    }
//  }
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