#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

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
  static void symbolize(Handle handle, const char* prefix);
  static void symbolizeMethod(Handle holder_name_handle,
                              Handle callee_name_handle);
  static void printSymExp(Handle handle);
  static void recordStmtObj(Handle stmt, Handle obj);
  static void recordPersistentObj(Handle obj);

  /* let compiler not optimize this  code!! */
  inline static bool __attribute__((optimize("O0"))) has_symbolized_method() {
    return ctx && ctx->is_symbolizing_method();
  }
  
  /* let compiler not optimize this  code!! */
  inline static bool __attribute__((optimize("O0"))) can_do_concolic() {
    return ctx && !ctx->is_symbolizing_method();
  }

  inline static bool __attribute__((optimize("O0"))) should_try_handle_method() {
    return ctx && ctx->get_method_symbolizer().has_handling_methods();
  }

  static bool has_callback() {
    return ctx && ctx->has_callback();
  }

  inline static void warning_reach_unhandled_bytecode(const char *bytecode) {
    if (can_do_concolic()) {
      tty->print_cr("[WARNING] reach unhandled bytecode %s!!!!", bytecode);
    }
  }

  /** unoptimize for breakpoint */
  inline static void __attribute__((optimize("O0"))) record_path_condition(Expression *sym_exp) {
    ctx->record_path_condition(sym_exp);

  }

  static void method_enter_callback(ZeroFrame *caller_frame, ZeroFrame *callee_frame) {

  }

  static void method_exit_callback(ZeroFrame *caller_frame) {
    ctx->get_method_symbolizer().method_exit(caller_frame);
  }
#else
  static jlong startConcolic();
  static jlong endConcolic();
  static void symbolize(Handle handle);
  static void symbolizeMethod(Handle holder_name_handle,
                              Handle callee_name_handle);
  static void recordStmtObj(Handle stmt, Handle obj) {};
  static void recordPersistentObj(Handle obj) {};
#endif // ENABLE_CONCOLIC
};

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP