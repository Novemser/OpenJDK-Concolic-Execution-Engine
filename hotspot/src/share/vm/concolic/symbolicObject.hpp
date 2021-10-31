#ifndef SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/symbolicExpression.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymbolicObject {
  static const int SYM_NAME_LENGTH = 8;
  typedef std::map<int, SymbolicExpression *> SymExpStore;

private:
  sym_oid_t _sym_oid;
  char _sym_name[SYM_NAME_LENGTH];
  SymExpStore _sym_exps;

public:
  SymbolicObject(sym_oid_t sym_oid) {
    _sym_oid = sym_oid;
    set_sym_name();
  }

  void set_sym_name() {
    int ret = sprintf(_sym_name, "S_%ld", _sym_oid);
    assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
  }

  char *get_sym_name() { return _sym_name; }

  void init_sym_exp(int field_index);

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP