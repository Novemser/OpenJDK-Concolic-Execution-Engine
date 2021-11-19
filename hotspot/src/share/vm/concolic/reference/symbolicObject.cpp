#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

SymObj::SymObj(sym_rid_t sym_rid) : SymInstance(sym_rid) {}

SymObj::~SymObj() {
  ExpStore::iterator iter;
  for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
    Expression *sym_exp = iter->second;
    if (sym_exp->dec_ref()) {
      delete sym_exp;
    }
  }
}

Expression *SymObj::get(int field_offset) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  ExpStore::iterator iter = _exps.find(field_offset);
  return iter == _exps.end() ? NULL : iter->second;
}

void SymObj::init_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  exp->inc_ref();
  _exps[field_offset] = exp;
}

void SymObj::set_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  ExpStore::iterator iter = _exps.find(field_offset);
  if (iter != _exps.end()) {
    Expression *old_exp = iter->second;
    if (old_exp && old_exp->dec_ref()) {
      delete old_exp;
    }
    if (exp) {
      exp->inc_ref();
      iter->second = exp;
    } else {
      _exps.erase(iter);
    }
  } else if (exp) {
    exp->inc_ref();
    _exps[field_offset] = exp;
  }
}

void SymObj::print() {
  ExpStore::iterator iter;
  for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
    tty->print_cr("Field(%d): ", iter->first);
    iter->second->print_cr();
  }
}

#endif