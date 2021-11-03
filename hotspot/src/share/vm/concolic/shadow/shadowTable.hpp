#ifndef SHARE_VM_CONCOLIC_SHADOW_SHADOWTABLE_HPP
#define SHARE_VM_CONCOLIC_SHADOW_SHADOWTABLE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"

#include <vector>

class ShadowTable {
public:
  struct Entry {
    Entry() { reset(); }

    sym_oid_t sym_oid; // for comparison
    int index;
    Expression *sym_exp;

    inline void reset() {
      sym_oid = 0;
      index = -1;
      sym_exp = NULL;
    }
  };

private:
  std::vector<Entry> _tbl;

public:
  ShadowTable();
  ~ShadowTable() {}

  void init(int max_slot_size);

  void set_slot(int offset, Expression *sym_exp, sym_oid_t sym_oid, int index) {
    Entry &entry = _tbl[offset];
    entry.sym_exp = sym_exp;
    entry.sym_oid = sym_oid;
    entry.index = index;
  }

  void clear_slot(int offset) { _tbl[offset].reset(); }

  Expression *get_slot(int offset) {
    Expression *ret = _tbl[offset].sym_exp;
    // assert(ret, "not null");
    return ret;
  }

  inline Entry& get_entry(int offset) {
    return _tbl[offset];
  }

  void copy_entries(ShadowTable &last_opr_stack, int src_begin, int dst_begin,
                    int size);

  inline int size() { return _tbl.size(); }

  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOW_SHADOWTABLE_HPP