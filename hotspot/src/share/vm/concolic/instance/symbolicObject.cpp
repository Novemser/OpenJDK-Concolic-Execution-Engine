#ifdef ENABLE_CONCOLIC

#include "concolic/instance/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

SymObj::SymObj(sym_oid_t sym_oid) : SymInstance(sym_oid) {}

SymObj::~SymObj() {
  ExpStore::iterator iter;
  for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
    Expression *sym_exp = iter->second;
    if (sym_exp->dec_ref()) {
      delete sym_exp;
    }
  }
}

Expression *SymObj::get(int field_index) {
  ExpStore::iterator iter = _exps.find(field_index);
  return iter == _exps.end() ? NULL : iter->second;
}

void SymObj::init_sym_exp(int field_index) {
  Expression *sym_exp =
      new SymbolExpression(this->get_sym_oid(), field_index);

  sym_exp->inc_ref();
  _exps[field_index] = sym_exp;
}

void SymObj::init_sym_exp(int field_index, Expression *exp) {
  exp->inc_ref();
  _exps[field_index] = exp;
}

void SymObj::set_sym_exp(int field_index, Expression *exp) {
  ExpStore::iterator iter = _exps.find(field_index);
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
    _exps[field_index] = exp;
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