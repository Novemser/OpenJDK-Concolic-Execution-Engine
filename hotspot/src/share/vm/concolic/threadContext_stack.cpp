#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"

/**
 * this will generate a new tmp_id; use it when `exp` is newly
 * calculated, which doesn't have `sym_rid` and `index`
 */
void ThreadContext::set_stack_slot(int offset, Expression *exp) {
  ShadowTable &opr_stack = this->get_last_stack();
  if (exp) {
    sym_tmp_id_t sym_tmp_id = this->get_next_sym_tmp_id(exp);
    opr_stack.set_slot(offset, exp, NULL_SYM_RID, sym_tmp_id);
  } else {
    opr_stack.clear_slot(offset);
  }
}

void ThreadContext::set_stack_slot_if_possible(int offset, Expression *exp) {
  if (this->get_last_stack().size() > 0) {
    this->set_stack_slot(offset, exp);
  }
}


void ThreadContext::set_stack_slot(int offset, Expression *exp,
                                   sym_rid_t sym_rid, int index) {
  ShadowTable &opr_stack = this->get_last_stack();
  if (exp) {
    opr_stack.set_slot(offset, exp, sym_rid, index);
  } else {
    opr_stack.clear_slot(offset);
  }
}

/**
 * TODO: document when to clear the stack
 */
void ThreadContext::clear_stack_slot(int offset) {
  return this->get_last_stack().clear_slot(offset);
}

void ThreadContext::copy_stack_slot(int src_off, int dst_off, int size) {
  ShadowTable &opr_stack = this->get_last_stack();
  opr_stack.copy_entries(opr_stack, src_off, dst_off, size);
}

void ThreadContext::swap_two_stack_slot(int off1, int off2) {
  ShadowTable &opr_stack = this->get_last_stack();
  opr_stack.swap_two_entries(off1, off2);
}

void ThreadContext::copy_entry_from_local_to_stack(int local_offset,
                                                   int stack_offset) {
  assert(local_offset >= 0, "local_offset >= 0");
  assert(stack_offset >= 0, "stack_offset >= 0");
  ShadowTable::Entry &entry = this->get_local_entry(local_offset);
  this->get_last_stack().set_slot(stack_offset, entry);
}

void ThreadContext::copy_entry_from_stack_to_local(int stack_offset,
                                                   int local_offset) {
  assert(local_offset >= 0, "local_offset >= 0");
  assert(stack_offset >= 0, "stack_offset >= 0");
  ShadowTable::Entry &entry = this->get_stack_entry(stack_offset);
  this->get_last_local().set_slot(local_offset, entry);
}

ArrayInternal *ThreadContext::get_or_create_array_internal(arrayOop oop) {
  if (!oop) {
    return NULL;
  }

  if (_array_store.find(oop) == _array_store.end()) {
    _array_store.insert(std::make_pair(oop, new ArrayInternal(oop->length())));
  }

  return _array_store.at(oop);
}


#endif