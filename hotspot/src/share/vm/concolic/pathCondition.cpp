#ifdef ENABLE_CONCOLIC

#include "concolic/pathCondition.hpp"
#include "utilities/ostream.hpp"

void PathCondition::add(Expression *exp) {
  exp->inc_ref();
  _exps.push_back(exp);
}

void PathCondition::gc() {
  int size = _exps.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _exps[i];
    if (exp->dec_ref()) {
      delete exp;
    }
  }
}

void PathCondition::print() {
  tty->print_cr("PathCondition:");
  int size = _exps.size();
  for (int i = 0; i < size; ++i) {
    tty->print_cr("- exp[%d]:", i);
    _exps[i]->print();
  }
}

#endif
