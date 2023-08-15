#ifdef ENABLE_CONCOLIC

#include "concolic/shadow/shadowTable.hpp"
#include "utilities/ostream.hpp"
#include "concolic/concolicMngr.hpp"

ShadowTable::ShadowTable() {}

void ShadowTable::init(int max_slot_size) { _tbl.resize(max_slot_size); }

void ShadowTable::copy_entries(ShadowTable &src_tbl, int src_begin,
                               int dst_begin, int size) {
  /**
   * TODO: memcpy
   */
  for (int i = 0; i < size; i++) {
    const Entry &entry = src_tbl.get_entry(src_begin++);
    set_slot(dst_begin++, entry);
  }
}

void ShadowTable::print() {
  int size = _tbl.size();
  tty->print_cr(" size: %d", size);
  for (int i = 0; i < size; ++i) {
    int offset = size - i - 1;
    Entry &entry = _tbl[offset];
    tty->print_cr("     [%d] SymExpression: %p, sym_rid: %lu, index %d", offset,
                  entry.exp, entry.sym_rid, entry.index);
  }
}

void ShadowTable::set_slot(int offset, Expression *exp, sym_rid_t sym_rid, int index) {
  Entry &entry = _tbl[offset];
  entry.exp = exp;
  entry.sym_rid = sym_rid;
  entry.index = index;
}

Expression *ShadowTable::get_slot(int offset) {
  Expression *ret = _tbl[offset].exp;
  // assert(ret, "not null");
  return ret;
}

void ShadowTable::set_slot(int offset, Expression *exp) {
  Entry &entry = _tbl[offset];
  entry.exp = exp;
}


ShadowTable::Entry &ShadowTable::Entry::operator=(const ShadowTable::Entry &other) {
  this->sym_rid = other.sym_rid;
  this->index = other.index;
  this->exp = other.exp;
  return *this;
}
#endif
