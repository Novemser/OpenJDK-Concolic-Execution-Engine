#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicExpression.hpp"
#include "utilities/ostream.hpp"

void SymbolicExpression::print() {
  if (_is_leaf) {
    tty->print("%s\n", _data.exp);
  } else {
    _data.left->print();
    tty->print("%s\n", SymbolicOpStr[(int)_op]);
    _data.right->print();
  }
}

#endif