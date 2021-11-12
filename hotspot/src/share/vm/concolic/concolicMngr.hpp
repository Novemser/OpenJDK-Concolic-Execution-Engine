#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "jni_zero.h"

#include "concolic/defs.hpp"
#include "concolic/threadContext.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "runtime/handles.hpp"

class ConcolicMngr {
#ifdef ENABLE_CONCOLIC
private:
  static bool is_doing_concolic;

public:
  static bool is_symbolizing_method;
  static ThreadContext *ctx;
  static MethodSymbolizer *method_sym;

  static jlong startConcolic(JavaThread *thread);
  static jlong endConcolic();
  static void symbolize(Handle handle);

  inline static bool has_symbolized_method() {
    return is_doing_concolic && is_symbolizing_method;
  }

  inline static bool can_do_concolic() {
    return is_doing_concolic && !is_symbolizing_method;
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
#endif // ENABLE_CONCOLIC
};

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP