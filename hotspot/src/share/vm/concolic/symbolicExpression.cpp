#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicExpression.hpp"
#include "utilities/ostream.hpp"

SymbolicExpression::SymbolicExpression(char *sym_name, int field_index)
    : _ref_count(0), _op(op_null), _is_leaf(true) {
  int ret = sprintf(_data.exp, "%s.%d", sym_name, field_index);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

SymbolicExpression::SymbolicExpression(SymbolicExpression *l,
                                       SymbolicExpression *r, SymbolicOp op)
    : _ref_count(0), _op(op), _is_leaf(false) {
  _data.left = l;
  _data.right = r;
  if (l) {
    l->inc_ref();
  }
  if (r) {
    r->inc_ref();
  }
}

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