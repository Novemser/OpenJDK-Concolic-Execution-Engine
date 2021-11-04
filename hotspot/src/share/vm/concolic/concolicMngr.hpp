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

  /**
   * this will generate a new tmp_id; use it when `sym_exp` is newly 
   * calculated, which doesn't have `sym_oid` and `index`
   */
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
      if (entry.sym_oid != NULL_SYM_OID) {
        ctx->detach_tmp_exp(entry.index);
      } else {
        assert(false, "not sym obj");
      }
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

  inline static void copy_stack_slot(int src_off, int dst_off, int size) {
    ShadowTable &opr_stack =
        ctx->get_shadow_stack().get_last_frame().get_opr_stack();
    opr_stack.copy_entries(opr_stack, src_off, dst_off, size);
  }

  inline static void swap_two_stack_slot(int off1, int off2) {
    ShadowTable &opr_stack =
        ctx->get_shadow_stack().get_last_frame().get_opr_stack();
    opr_stack.swap_two_entries(off1, off2);
  }


  inline static void set_local_slot(int offset, Expression *sym_exp) {
    assert(offset >= 0, "offset >= 0");
    sym_tmp_id_t sym_tmp_id = ctx->get_next_sym_tmp_id(sym_exp);
    ctx->get_shadow_stack().get_last_frame().get_local_tbl().set_slot(
        offset, sym_exp, NULL_SYM_OID, sym_tmp_id);
  }

  inline static void set_local_slot(int offset, Expression *sym_exp,
                                    sym_oid_t sym_oid, int index) {
    ShadowTable &local_tbl =
        ctx->get_shadow_stack().get_last_frame().get_local_tbl();
    if (sym_exp) {
      local_tbl.set_slot(offset, sym_exp, sym_oid, index);
    } else {
      local_tbl.clear_slot(offset);
    }
  }

  inline static Expression *get_local_slot(int offset) {
    return ctx->get_shadow_stack().get_last_frame().get_local_tbl().get_slot(
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