#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

SymbolicObject::SymbolicObject(sym_oid_t sym_oid) {
  _sym_oid = sym_oid;
  set_sym_name();
}

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

void SymbolicObject::init_sym_exp(int field_index) {
  Expression *sym_exp =
      new FieldSymExpression(this->get_sym_name(), field_index);
  
  sym_exp->inc_ref();
  _sym_exps[field_index] = sym_exp;
}

void SymbolicObject::set_sym_exp(int field_index, Expression *sym_exp) {
  sym_exp->inc_ref();

  Expression *&target_sym_exp = _sym_exps[field_index];
  if (target_sym_exp) {
    if (target_sym_exp->dec_ref()) {
      delete target_sym_exp;
    }
  }
  target_sym_exp = sym_exp;
}

void SymbolicObject::print() {
  SymExpStore::iterator sym_exp_iter;
  for (sym_exp_iter = _sym_exps.begin(); sym_exp_iter != _sym_exps.end();
       ++sym_exp_iter) {
    sym_exp_iter->second->print();
  }
}

#endif