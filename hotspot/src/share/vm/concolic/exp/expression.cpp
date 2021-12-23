#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

ulong Expression::total_count = 0;

void Expression::print() { tty->indent().print("ref_count: %u", _ref_count); }

void Expression::print_cr() {
  print();
  tty->cr();
}

OpSymExpression::OpSymExpression(Expression *l, Expression *r, SymbolicOp op,
                                 bool cmp)
    : _left(l), _right(r), _op(cmp ? op : NotSymbolicOp[op]) {
  if (_left) {
    _left->inc_ref();
  }
  if (_right) {
    _right->inc_ref();
  }
  // tty->print_cr("Create: %p -> %p, %p", this, _left, _right);
}

OpSymExpression::OpSymExpression(Expression *r, SymbolicOp op)
    : _left(NULL), _right(r), _op(op) {
  if (_right) {
    _right->inc_ref();
  }
  // tty->print_cr("Create: %p -> %p, %p", this, _left, _right);
}

OpSymExpression::~OpSymExpression() {
  Expression::gc(_left);
  Expression::gc(_right);
}

void OpSymExpression::print() {
  tty->print("(%c %s ", (_left != NULL) ? '2' : '1', SymbolicOpStr[(int) _op]);
  if (_left) {
    _left->print();
    tty->print(" ");
  }
  Expression::print_on_maybe_null(_right);
  tty->print(")");
}

#endif