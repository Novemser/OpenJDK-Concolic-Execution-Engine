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
    /**
     * TODO: shall we still use the name `index`? Although we changed to offset 
     * for symbolic object, this field (`index`) is also used for tmp_sym's index. 
     * So I didn't change its name.
     */
    int index;
    Expression *sym_exp;

    inline void reset() {
      sym_oid = 0;
      index = -1;
      sym_exp = NULL;
    }

    Entry &operator=(const Entry &other) {
      this->sym_oid = other.sym_oid;
      this->index = other.index;
      this->sym_exp = other.sym_exp;
      return *this;
    }
  };

private:
  std::vector<Entry> _tbl;

public:
  ShadowTable();
  ~ShadowTable() {}

  void init(int max_slot_size);

  void set_slot(int offset, const Entry &other) {
    _tbl[offset] = other;
  }

  void set_slot(int offset, Expression *sym_exp, sym_oid_t sym_oid, int index) {
    Entry &entry = _tbl[offset];
    entry.sym_exp = sym_exp;
    entry.sym_oid = sym_oid;
    entry.index = index;
  }

  void set_slot(int offset, Expression *sym_exp) {
    Entry &entry = _tbl[offset];
    entry.sym_exp = sym_exp;
  }

  void clear_slot(int offset) { _tbl[offset].reset(); }

  Expression *get_slot(int offset) {
    Expression *ret = _tbl[offset].sym_exp;
    // assert(ret, "not null");
    return ret;
  }

  inline Entry &get_entry(int offset) { return _tbl[offset]; }

  void copy_entries(ShadowTable &last_opr_stack, int src_begin, int dst_begin,
                    int size);

  void swap_two_entries(int off1, int off2) {
    Entry &entry1 = _tbl[off1];
    Entry &entry2 = _tbl[off2];
    Entry temp;

    temp = entry1;
    entry1 = entry2;
    entry2 = temp;
  }

  inline int size() { return _tbl.size(); }

  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOW_SHADOWTABLE_HPP