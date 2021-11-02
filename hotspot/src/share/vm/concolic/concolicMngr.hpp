#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "jni_zero.h"

#include "concolic/defs.hpp"
#include "concolic/threadContext.hpp"
#include "runtime/handles.hpp"

class ConcolicMngr {
public:
  static bool is_doing_concolic;
  static ThreadContext *ctx;

  static jlong startConcolic(JavaThread *thread);
  static jlong endConcolic();
  static void symbolize(Handle handle);

  inline static void set_stack_slot(int offset, SymbolicExpression *sym_exp) {
    assert(offset > 0, "offset > 0");
    tmp_id_t tmp_id = ctx->get_next_tmp_id();
    ctx->get_shadow_stack().get_last_frame().get_opr_stack().set_slot(
        offset, sym_exp, NULL_SYM_OID, tmp_id);
  }

  inline static void set_stack_slot(int offset, SymbolicExpression *sym_exp,
                                    sym_oid_t sym_oid, int index) {
    ctx->get_shadow_stack().get_last_frame().get_opr_stack().set_slot(
        offset, sym_exp, sym_oid, index);
  }

  inline static SymbolicExpression *get_stack_slot(int offset) {
    return ctx->get_shadow_stack().get_last_frame().get_opr_stack().get_slot(
        offset);
  }
};

/**
 * Macro for CONCOLIC_DEBUG_BLOCK
 */
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_DEBUG)
#define CONCOLIC_DEBUG_BLOCK_BEGIN if (ConcolicMngr::is_doing_concolic) {
#else
#define CONCOLIC_DEBUG_BLOCK_BEGIN if (false) {
#endif

#define CONCOLIC_DEBUG_BLOCK_END }

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP