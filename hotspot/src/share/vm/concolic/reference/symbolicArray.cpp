#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicArray.hpp"
#include "utilities/ostream.hpp"

SymArr::SymArr(sym_rid_t sym_rid, Expression *length_exp)
    : SymRef(sym_rid), _version(0), _load_count(0),
      _length_exp(length_exp) {
  if (_length_exp) {
    _length_exp->inc_ref();
  }
}

SymArr::~SymArr() {
  Expression::gc(_length_exp);
}

void SymArr::print() {
  tty->print_cr("SymArr: ");
  if (_length_exp) {
    _length_exp->print();
  }
}

#endif