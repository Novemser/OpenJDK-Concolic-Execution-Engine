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

ConExpression::ConExpression(jboolean z) {
  int ret = sprintf(_str, "Y_Z_0x%x", z);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jbyte b) {
  int ret = sprintf(_str, "Y_B_0x%x", b);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jchar b) {
  int ret = sprintf(_str, "Y_C_%c", b);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jint i) {
  int ret = sprintf(_str, "Y_I_0x%x", i);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jshort s) {
  int ret = sprintf(_str, "Y_S_0x%x", s);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jlong l) {
  int ret = sprintf(_str, "Y_J_0x%lx", l);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jfloat f) {
  int ret = sprintf(_str, "Y_F_%e", f);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jdouble d) {
  int ret = sprintf(_str, "Y_D_%le", d);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

void ConExpression::print() {
  // TODO: include primiteive type
  tty->indent().print("%s", _str);
}

#endif