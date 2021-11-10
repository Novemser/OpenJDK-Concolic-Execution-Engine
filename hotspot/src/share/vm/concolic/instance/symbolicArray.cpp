#ifdef ENABLE_CONCOLIC

#include "concolic/instance/symbolicArray.hpp"
#include "utilities/ostream.hpp"

SymArr::SymArr(sym_oid_t sym_oid, Expression *length_exp)
    : SymInstance(sym_oid), _length_exp(length_exp) {
  if (_length_exp) {
    _length_exp->inc_ref();
  }
}

SymArr::~SymArr() {
  if (_length_exp && _length_exp->dec_ref()) {
    delete _length_exp;
  }
}

void SymArr::store() {
  ++_version;
  _load_count = 0;
}

void SymArr::print() {
  tty->print_cr("SymArr: ");
  if (_length_exp) {
    _length_exp->print();
  }
}

#endif