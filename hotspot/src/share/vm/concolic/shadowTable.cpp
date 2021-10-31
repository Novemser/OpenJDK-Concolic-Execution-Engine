#ifdef ENABLE_CONCOLIC

#include "concolic/shadowTable.hpp"

#include <algorithm>

ShadowTable::ShadowTable() {}

void ShadowTable::init(int max_slot_size) { _tbl.resize(max_slot_size); }

void ShadowTable::print() { tty->print_cr(" size: %lu", _tbl.size()); }

#endif
