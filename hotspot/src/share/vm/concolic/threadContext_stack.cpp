#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"

/**
 * this will generate a new tmp_id; use it when `sym_exp` is newly
 * calculated, which doesn't have `sym_oid` and `index`
 */
void ThreadContext::set_stack_slot(int offset, Expression *sym_exp) {
  assert(offset >= 0, "offset >= 0");
  sym_tmp_id_t sym_tmp_id = this->get_next_sym_tmp_id(sym_exp);
  this->get_shadow_stack().get_last_frame().get_opr_stack().set_slot(
      offset, sym_exp, NULL_SYM_OID, sym_tmp_id);
}

void ThreadContext::set_stack_slot(int offset, Expression *sym_exp,
                                   sym_oid_t sym_oid, int index) {
  ShadowTable &opr_stack =
      this->get_shadow_stack().get_last_frame().get_opr_stack();
  if (sym_exp) {
    opr_stack.set_slot(offset, sym_exp, sym_oid, index);
  } else {
    opr_stack.clear_slot(offset);
  }
}

Expression *ThreadContext::get_stack_slot(int offset) {
  return this->get_shadow_stack().get_last_frame().get_opr_stack().get_slot(
      offset);
}

Expression *ThreadContext::get_stack_slot_and_detach(int offset) {
  ShadowTable &opr_stack =
      this->get_shadow_stack().get_last_frame().get_opr_stack();
  ShadowTable::Entry &entry = opr_stack.get_entry(offset);
  if (entry.sym_exp) {
    if (entry.sym_oid == NULL_SYM_OID) {
      this->detach_tmp_exp(entry.index);
    }
  }
  return entry.sym_exp;
}

/**
 * TODO: document when to clear the stack
 */
void ThreadContext::clear_stack_slot(int offset) {
  return this->get_shadow_stack().get_last_frame().get_opr_stack().clear_slot(
      offset);
}

void ThreadContext::copy_stack_slot(int src_off, int dst_off, int size) {
  ShadowTable &opr_stack =
      this->get_shadow_stack().get_last_frame().get_opr_stack();
  opr_stack.copy_entries(opr_stack, src_off, dst_off, size);
}

void ThreadContext::swap_two_stack_slot(int off1, int off2) {
  ShadowTable &opr_stack =
      this->get_shadow_stack().get_last_frame().get_opr_stack();
  opr_stack.swap_two_entries(off1, off2);
}

void ThreadContext::copy_entry_from_local_to_stack(int local_offset,
                                                   int stack_offset) {
  assert(local_offset >= 0, "local_offset >= 0");
  assert(stack_offset >= 0, "stack_offset >= 0");
  ShadowTable::Entry &entry =
      this->get_shadow_stack().get_last_frame().get_local_tbl().get_entry(
          local_offset);
  this->get_shadow_stack().get_last_frame().get_opr_stack().set_slot(
      stack_offset, entry);
}

void ThreadContext::copy_entry_from_stack_to_local(int stack_offset,
                                                   int local_offset) {
  assert(local_offset >= 0, "local_offset >= 0");
  assert(stack_offset >= 0, "stack_offset >= 0");
  ShadowTable::Entry &entry =
      this->get_shadow_stack().get_last_frame().get_opr_stack().get_entry(
          stack_offset);
  this->get_shadow_stack().get_last_frame().get_local_tbl().set_slot(
      local_offset, entry);
}

Expression *ThreadContext::get_local_slot(int offset) {
  return this->get_shadow_stack().get_last_frame().get_local_tbl().get_slot(
      offset);
}

void ThreadContext::set_local_slot(int offset, Expression *sym_exp) {
  assert(offset >= 0, "offset >= 0");
  this->get_shadow_stack().get_last_frame().get_local_tbl().set_slot(offset,
                                                                     sym_exp);
}

#endif