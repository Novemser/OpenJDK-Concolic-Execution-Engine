#ifndef SHARE_VM_CONCOLIC_SHADOWTABLE_HPP
#define SHARE_VM_CONCOLIC_SHADOWTABLE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/symbolicExpression.hpp"

#include <vector>

class ShadowTable {
private:
  struct Entry {
    Entry() {
      sym_oid = 0;
      index = -1;
      sym_exp = NULL;
    }

    sym_oid_t sym_oid; // for comparison
    int index;
    SymbolicExpression *sym_exp;
  };

  std::vector<Entry> _tbl;

public:
  ShadowTable();
  ~ShadowTable() {}

  void init(int max_slot_size);

  void set_slot(int offset, SymbolicExpression *sym_exp, sym_oid_t sym_oid, int index) {
    Entry& entry = _tbl[offset];
    entry.sym_exp = sym_exp;
    entry.sym_oid = sym_oid;
    entry.index = index;
  }

  SymbolicExpression *set_slot(int offset) {
    SymbolicExpression *ret = _tbl[offset].sym_exp;
    assert(ret, "not null");
    return ret;
  }

  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOWTABLE_HPP