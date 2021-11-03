#ifdef ENABLE_CONCOLIC

#include "concolic/shadow/shadowTable.hpp"
#include "utilities/ostream.hpp"

ShadowTable::ShadowTable() {}

void ShadowTable::init(int max_slot_size) { _tbl.resize(max_slot_size); }


void ShadowTable::copy_entries(ShadowTable &last_opr_stack, int begin_offset, int end_offset) {
  /**
   * TODO: memcpy
   */
  for (int offset = begin_offset; offset < end_offset; offset++) {
    Entry &entry = last_opr_stack.get_entry(offset);
    set_slot(offset - begin_offset, entry.sym_exp, entry.sym_oid, entry.index);
  }
}

void ShadowTable::print() {
  int size = _tbl.size();
  tty->print_cr(" size: %d", size);
  for (int i = 0; i < size; ++i) {
    int offset = size - i - 1;
    Entry &entry = _tbl[offset];
    tty->print_cr("     [%d] SymbolicExpression: %p, sym_oid: %lu, index %d",
                  offset, entry.sym_exp, entry.sym_oid, entry.index);
  }
}

#endif
