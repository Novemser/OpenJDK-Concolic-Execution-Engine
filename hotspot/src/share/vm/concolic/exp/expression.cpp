#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

ulong Expression::total_count = 0;

void Expression::print() { tty->print_cr("ref_count: %u", _ref_count); };

FieldSymExpression::FieldSymExpression(sym_oid_t sym_oid, int field_index) {
  int ret = sprintf(_str, "S_%lu.%d", sym_oid, field_index);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

FieldSymExpression::~FieldSymExpression() {}

void FieldSymExpression::print() { tty->print("%s\n", _str); }

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
  if (_left && _left->dec_ref()) {
    delete _left;
  }
  if (_right && _right->dec_ref()) {
    delete _right;
  }
}

void OpSymExpression::print() {
  if (_left) {
    _left->print();
  }
  tty->print_cr("%s", SymbolicOpStr[(int)_op]);
  _right->print();
}

ConExpression::ConExpression(jint i) {
  int ret = sprintf(_str, "%d", i);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jlong l) {
  int ret = sprintf(_str, "%ld", l);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jfloat f) {
  int ret = sprintf(_str, "%f", f);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jdouble d) {
  int ret = sprintf(_str, "%lf", d);
  assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
}

void ConExpression::print() { tty->print("C_%s\n", _str); }

#endif