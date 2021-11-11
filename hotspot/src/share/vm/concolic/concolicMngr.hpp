#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "jni_zero.h"

#include "concolic/defs.hpp"
#include "concolic/threadContext.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "runtime/handles.hpp"

class ConcolicMngr {
public:

#ifdef ENABLE_CONCOLIC

  static bool is_doing_concolic;
  static ThreadContext *ctx;
  static MethodSymbolizer *method_sym;

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
      if (entry.sym_oid == NULL_SYM_OID) {
        ctx->detach_tmp_exp(entry.index);
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

  inline static void copy_entry_from_local_to_stack(int local_offset, int stack_offset) {
    assert(local_offset >= 0, "local_offset >= 0");
    assert(stack_offset >= 0, "stack_offset >= 0");
    ShadowTable::Entry &entry = 
        ctx->get_shadow_stack().get_last_frame().get_local_tbl().get_entry(local_offset);
    ctx->get_shadow_stack().get_last_frame().get_opr_stack().set_slot(
        stack_offset, entry);
  }

  inline static void copy_entry_from_stack_to_local(int stack_offset, int local_offset) {
    assert(local_offset >= 0, "local_offset >= 0");
    assert(stack_offset >= 0, "stack_offset >= 0");
    ShadowTable::Entry &entry = 
        ctx->get_shadow_stack().get_last_frame().get_opr_stack().get_entry(stack_offset);
    ctx->get_shadow_stack().get_last_frame().get_local_tbl().set_slot(
        local_offset, entry);
  }

  inline static Expression *get_local_slot(int offset) {
    return ctx->get_shadow_stack().get_last_frame().get_local_tbl().get_slot(
        offset);
  }

  inline static void set_local_slot(int offset, Expression *sym_exp) {
    assert(offset >= 0, "offset >= 0");
    ctx->get_shadow_stack().get_last_frame().get_local_tbl().set_slot(offset, sym_exp);
  }


        
  inline static void warning_reach_unhandled_bytecode(const char *bytecode) {
    if (is_doing_concolic) {
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