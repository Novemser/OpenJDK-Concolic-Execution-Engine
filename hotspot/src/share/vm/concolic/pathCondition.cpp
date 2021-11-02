#ifdef ENABLE_CONCOLIC

#include "concolic/pathCondition.hpp"
#include "utilities/ostream.hpp"

void PathCondition::add(SymbolicExpression *sym_exp) {
  _sym_exps.push_back(sym_exp);
}

void PathCondition::print() {
  tty->print_cr("PathCondition:");
  int size = _sym_exps.size();
  for (int i = 0; i < size; ++i) {
    tty->print_cr("- sym_exp[%d]:", i);
    _sym_exps[i]->print();
  }
}

#endif
