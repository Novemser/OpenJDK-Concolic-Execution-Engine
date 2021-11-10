#ifndef SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICARRAY_HPP
#define SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICARRAY_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/instance/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymArr : public SymInstance {
private:
  int _version;
  int _load_count;
  Expression *_length_exp;

public:
  SymArr(sym_oid_t sym_oid, Expression *length_exp);
  ~SymArr();

  inline void store() {
    ++_version;
    _load_count = 0;
  }

  inline void set_length_exp(Expression *length_exp) {
    assert(_length_exp == NULL, "not override old one");
    assert(length_exp != NULL, "not NULL");
    _length_exp = length_exp;
    _length_exp->inc_ref();
  }

  inline Expression *get_length_exp() { return _length_exp; }
  inline int get_version() { return _version; }
  inline int get_and_inc_load_count() { return _load_count++; }

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_INSTANCE_SYMBOLICARRAY_HPP