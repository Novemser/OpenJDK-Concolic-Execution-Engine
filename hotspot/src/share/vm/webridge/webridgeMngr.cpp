//
// Created by gansen on 4/21/22.
//

#include <iostream>
#include "webridgeMngr.hpp"

#ifdef ENABLE_WEBRIDGE

#include "jni.h"
#include "runtime/interfaceSupport.hpp"

void webridgeMngr::analyse(ThreadContext *ctx) {
  if (!ctx) {
    tty->print_cr("[WeBridge] No associated thread context found, concolic execution might not enabled!");
    return;
  }

  JdbcMngr jdbc_mgr = ctx->get_jdbc_mngr();
  const std::vector<std::pair<SymStmt *, jlong> > &sym_stmt_list =
      jdbc_mgr.get_sym_stmt_list();
  tty->print_cr("[WeBridge] Received %ld SQL Statements", sym_stmt_list.size());
  JavaVM *jvm;
  JNIEnv *env;

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

  jvm->GetEnv((void **) &env, JNI_VERSION_1_8);
  if (env == NULL) {
    tty->print_cr("JNI env not initialized. Exiting WeBridge");
    return;
  }

  // 0. Calling version
  jint ver = env->GetVersion();
  tty->print_cr("JVM load successded: version %d.%d", (ver >> 16) & 0x0f, ver & 0x0f);
//  ResourceMark rm(Thread::current());
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

#endif