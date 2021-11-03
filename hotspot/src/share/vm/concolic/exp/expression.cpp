#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

void Expression::print() {
  tty->print_cr("ref_count: %u", _ref_count);
};

FieldSymbolicExpression::FieldSymbolicExpression(char *sym_name,
                                                 int field_index) {
  int ret = sprintf(_sym_str, "%s.%d", sym_name, field_index);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

void FieldSymbolicExpression::print() { tty->print("%s\n", _sym_str); }

OpSymbolicExpression::OpSymbolicExpression(Expression *l, Expression *r,
                                           SymbolicOp op, bool cmp)
    : _op(cmp ? op : NotSymbolicOp[op]) {
  if (l) {
    l->inc_ref();
    _left = l;
  }
  if (r) {
    r->inc_ref();
    _right = r;
  }
}

void OpSymbolicExpression::print() {
  _left->print();
  tty->print("%s\n", SymbolicOpStr[(int)_op]);
  _right->print();
}

#endif