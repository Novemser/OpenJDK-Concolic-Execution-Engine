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

  inline static void set_stack_slot(int offset, Expression *sym_exp) {
    assert(offset >= 0, "offset >= 0");
    sym_tmp_id_t sym_tmp_id = ctx->get_next_sym_tmp_id(sym_exp);
    ctx->get_shadow_stack().get_last_frame().get_opr_stack().set_slot(
        offset, sym_exp, NULL_SYM_OID, sym_tmp_id);
  }

  inline static void set_stack_slot(int offset, Expression *sym_exp,
                                    sym_oid_t sym_oid, int index) {
    ShadowTable &opr_stack =
        ctx->get_shadow_stack().get_last_frame().get_opr_stack();
    if (sym_exp) {
      opr_stack.set_slot(offset, sym_exp, sym_oid, index);
    } else {
      opr_stack.clear_slot(offset);
    }
  }

  inline static Expression *get_stack_slot(int offset) {
    return ctx->get_shadow_stack().get_last_frame().get_opr_stack().get_slot(
        offset);
  }

  inline static Expression *get_stack_slot_and_detach(int offset) {
    ShadowTable &opr_stack =
        ctx->get_shadow_stack().get_last_frame().get_opr_stack();
    ShadowTable::Entry &entry = opr_stack.get_entry(offset);
    if (entry.sym_exp) {
      assert(entry.sym_oid == NULL_SYM_OID, "not sym obj");
      ctx->detach_tmp_exp(entry.index);
    }
    return entry.sym_exp;
  }

  /**
   * TODO: document when to clear the stack
   */
  inline static void clear_stack_slot(int offset) {
    return ctx->get_shadow_stack().get_last_frame().get_opr_stack().clear_slot(
        offset);
  }

  inline static void record_path_condition(Expression *sym_exp) {
    ctx->record_path_condition(sym_exp);
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