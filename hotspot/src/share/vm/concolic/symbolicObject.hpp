#ifndef SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/symbolicExpression.hpp"
#include "runtime/handles.hpp"
#include "utilities/debug.hpp"

#include <stdio.h>
#include <map>

class SymbolicObject {
  static const int SYM_NAME_LENGTH = 8;
  typedef std::map<int, SymbolicExpression *> ExpStore;

private:
  sym_oid_t _sym_oid;
  char sym_name[SYM_NAME_LENGTH];

public:
  SymbolicObject(sym_oid_t sym_oid) {
    _sym_oid = sym_oid;

    set_sym_name();
  }

  ~SymbolicObject() {}

  void set_sym_name() {
    int ret = sprintf(sym_name, "S_%ld", _sym_oid);
    assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
  }

  char *get_sym_name() { return sym_name; }

  void symbolize(Handle handle);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP