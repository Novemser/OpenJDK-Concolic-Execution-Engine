#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void SymbolicObject::init_sym_exp(int field_index) {
  SymbolicExpression *sym_exp =
      new SymbolicExpression(this->get_sym_name(), field_index);

  _sym_exps.insert(std::make_pair(field_index, sym_exp));
}

void SymbolicObject::set_sym_exp(int field_index, SymbolicExpression *sym_exp) {
  _sym_exps.insert(std::make_pair(field_index, sym_exp));
}

void SymbolicObject::print() {
  SymExpStore::iterator sym_exp_iter;
  for (sym_exp_iter = _sym_exps.begin(); sym_exp_iter != _sym_exps.end();
       ++sym_exp_iter) {
    sym_exp_iter->second->print();
  }
}

#endif