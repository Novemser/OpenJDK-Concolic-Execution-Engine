#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "jni_md.h"

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "concolic/threadContext.hpp"
#include "runtime/handles.hpp"

#include <pthread.h>

class ConcolicMngr {
#ifdef ENABLE_CONCOLIC
public:
  static volatile int _num_threads_in_concolic;
  static pthread_mutex_t mutex;
  static __thread ThreadContext *ctx;

  static jlong startConcolic(JavaThread *thread);
  static jlong endConcolic();
  static void symbolize(Handle handle);
  static void symbolizeMethod(Handle holder_name_handle,
                              Handle callee_name_handle);

  /* let compiler not optimize this  code!! */
  inline static bool __attribute__((optimize("O0"))) has_symbolized_method() {
    return ctx && ctx->is_symbolizing_method();
  }
  
  /* let compiler not optimize this  code!! */
  inline static bool __attribute__((optimize("O0"))) can_do_concolic() {
    return ctx && !ctx->is_symbolizing_method();
  }

  inline static void warning_reach_unhandled_bytecode(const char *bytecode) {
    if (can_do_concolic()) {
      tty->print_cr("[WARNING] reach unhandled bytecode %s!!!!", bytecode);
    }
  }

  inline static void record_path_condition(Expression *sym_exp) {
    ctx->record_path_condition(sym_exp);
  }
#else
  static jlong startConcolic();
  static jlong endConcolic();
  static void symbolize(Handle handle);
  static void symbolizeMethod(Handle holder_name_handle,
                              Handle callee_name_handle);
#endif // ENABLE_CONCOLIC
};

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP