#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "utilities/ostream.hpp"

ulong Expression::total_count = 0;

void Expression::print() { tty->indent().print("ref_count: %u", _ref_count); }

void Expression::print_cr() {
  print();
  tty->cr();
}

SymbolExpression::SymbolExpression(sym_oid_t sym_oid, int field_index) {
  int ret;
  if (field_index == NULL_INDEX) {
    ret = sprintf(_str, "S_%lu", sym_oid);
  } else {
    ret = sprintf(_str, "S_%lu.%d", sym_oid, field_index);
  }
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

SymbolExpression::SymbolExpression(sym_oid_t sym_arr_oid, int version,
                                   int load_count) {
  int ret= sprintf(_str, "A_%lu-%d-%d", sym_arr_oid, version, load_count);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

SymbolExpression::~SymbolExpression() {}

void SymbolExpression::print() { tty->indent().print("%s", _str); }

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
#ifdef ONELINE
  tty->print("(%s ", SymbolicOpStr[(int)_op]);
  if (_left) {
    _left->print();
    tty->print(" ");
  }
  _right->print();
  tty->print(")");
#else
  int pos = tty->indentation();
  tty->indent().print("(");
  // tty->set_indentation(tty->position());
  // tty->indent().print("%s ", SymbolicOpStr[(int)_op]);
  tty->indent().print_cr(SymbolicOpStr[(int)_op]);
  tty->inc(2);
  if (_left) {
    _left->print();
    tty->cr();
  }
  _right->print();
  tty->cr();
  tty->set_indentation(pos);
  tty->indent().print(")");
#endif
}

ConExpression::ConExpression(jint i) {
  int ret = sprintf(_str, "0x%x", i);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jlong l) {
  int ret = sprintf(_str, "0x%lx", l);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jfloat f) {
  int ret = sprintf(_str, "%e", f);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jdouble d) {
  int ret = sprintf(_str, "%e", d);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

void ConExpression::print() {
  // TODO: include primiteive type
  tty->indent().print("C_%s", _str);
}

#endif