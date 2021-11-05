#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

SymbolicObject::SymbolicObject(sym_oid_t sym_oid) : _sym_oid(sym_oid) {}

SymbolicObject::~SymbolicObject() {
  SymExpStore::iterator sym_exp_iter;
  for (sym_exp_iter = _sym_exps.begin(); sym_exp_iter != _sym_exps.end();
       ++sym_exp_iter) {
    Expression *sym_exp = sym_exp_iter->second;
    if (sym_exp->dec_ref()) {
      delete sym_exp;
    }
  }
}
Expression *SymbolicObject::get(int field_index) {
  SymExpStore::iterator iter = _sym_exps.find(field_index);
  return iter == _sym_exps.end() ? NULL : iter->second;
}

void SymbolicObject::init_sym_exp(int field_index) {
  Expression *sym_exp =
      new FieldSymExpression(this->get_sym_oid(), field_index);

  sym_exp->inc_ref();
  _sym_exps[field_index] = sym_exp;
}

void SymbolicObject::init_sym_exp(int field_index, Expression *exp) {
  exp->inc_ref();
  _sym_exps[field_index] = exp;
}

void SymbolicObject::set_sym_exp(int field_index, Expression *sym_exp) {
  SymExpStore::iterator iter = _sym_exps.find(field_index);
  if (iter != _sym_exps.end()) {
    Expression *old_exp = iter->second;
    if (old_exp && old_exp->dec_ref()) {
      delete old_exp;
    }
    if (sym_exp) {
      sym_exp->inc_ref();
      iter->second = sym_exp;
    } else {
      _sym_exps.erase(iter);
    }
  } else if (sym_exp) {
    sym_exp->inc_ref();
    _sym_exps[field_index] = sym_exp;
  }
}

void SymbolicObject::print() {
  SymExpStore::iterator sym_exp_iter;
  for (sym_exp_iter = _sym_exps.begin(); sym_exp_iter != _sym_exps.end();
       ++sym_exp_iter) {
    tty->print_cr("Field(%d): ", sym_exp_iter->first);
    sym_exp_iter->second->print_cr();
  }
}

#endif