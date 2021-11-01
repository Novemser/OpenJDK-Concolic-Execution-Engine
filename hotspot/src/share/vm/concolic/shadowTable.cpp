#ifdef ENABLE_CONCOLIC

#include "concolic/shadowTable.hpp"

#include <algorithm>

ShadowTable::ShadowTable() {}

void ShadowTable::init(int max_slot_size) { _tbl.resize(max_slot_size); }

void ShadowTable::print() {
  int size = _tbl.size();
  tty->print_cr(" size: %d", size);
  for (int i = 0; i < size; ++i) {
    Entry &entry = _tbl[i];
    tty->print_cr("     [%d] SymbolicExpression: %p, sym_oid: %lu, index %d", i,
                  entry.sym_exp, entry.sym_oid, entry.index);
  }
}

#endif
