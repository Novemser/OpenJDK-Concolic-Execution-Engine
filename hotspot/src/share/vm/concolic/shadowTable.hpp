#ifndef SHARE_VM_CONCOLIC_SHADOWTABLE_HPP
#define SHARE_VM_CONCOLIC_SHADOWTABLE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicExpression.hpp"

#include <vector>

class ShadowTable {
private:
  struct Entry {
    Entry() {
      ptr = NULL;
      sym_exp = NULL;
    }

    intptr_t *ptr; // for comparison
    SymbolicExpression *sym_exp;
  };

  std::vector<Entry> _tbl;

public:
  ShadowTable();
  ~ShadowTable() {}

  void init(int max_slot_size);

	void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOWTABLE_HPP