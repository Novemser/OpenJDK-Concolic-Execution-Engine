#ifndef SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymbolicObject {
  static const int SYM_NAME_LENGTH = 8;
  typedef std::map<int, Expression *> SymExpStore;

private:
  sym_oid_t _sym_oid;
  char _sym_name[SYM_NAME_LENGTH];
  SymExpStore _sym_exps;

public:
  SymbolicObject(sym_oid_t sym_oid);
  ~SymbolicObject();

  void set_sym_name() {
    int ret = sprintf(_sym_name, "S_%ld", _sym_oid);
    assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
  }

  Expression *get(int field_index);

  char *get_sym_name() { return _sym_name; }

  void init_sym_exp(int field_index);
  void set_sym_exp(int field_index, Expression *sym_exp);

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP