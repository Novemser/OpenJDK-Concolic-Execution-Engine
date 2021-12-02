#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICARRAY_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICARRAY_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/reference/symbolicReference.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymArr : public SymRef {
private:
  int _version;
  int _load_count;
  Expression *_length_exp;

public:
  SymArr(sym_rid_t sym_rid, Expression *length_exp);
  ~SymArr();

  Expression *get_ref_exp() { return NULL; };

  inline void store() {
    ++_version;
    _load_count = 0;
  }

  inline void set_length_exp(Expression *length_exp) {
    Expression::gc(_length_exp);
    _length_exp = length_exp;
    _length_exp->inc_ref();
  }

  inline Expression *get_length_exp() { return _length_exp; }
  inline int get_version() { return _version; }
  inline int get_load_count() { return _load_count; }
  inline int get_and_inc_load_count() { return ++_load_count; }

public:
  void print();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICARRAY_HPP