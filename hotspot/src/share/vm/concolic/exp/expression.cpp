#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "concolic/concolicMngr.hpp"
#include "utilities/ostream.hpp"

ulong Expression::total_count = 0;

Expression::Expression() : _ref_count(0) {
  total_count++;
}

void Expression::print() { tty->indent().print("ref_count: %u at code %s", _ref_count, _java_code_position.c_str()); }

void Expression::print_cr() {
  print();
  tty->cr();
}

void Expression::set_java_code_position(const std::string &javaCodePosition) {
  _java_code_position = javaCodePosition;
}

const std::string &Expression::getJavaCodePosition() const {
  return _java_code_position;
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

void OpSymExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) const {
  writer.Key("_type");
  writer.String("BinaryExpression");

  writer.Key("_left");
  if (_left) {
    _left->serialize(writer);
  } else {
    writer.Null();
  }

  writer.Key("_op");
  writer.String(SymbolicOpStr[(int) _op]);

  writer.Key("_right");
  if (_right) {
    _right->serialize(writer);
  } else {
    writer.Null();
  }
}

ConExpression::ConExpression(jboolean z) {
  _type = type2char(T_BOOLEAN);
  int ret = sprintf(_str, "Y_Z_%u", z);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jbyte b) {
  _type = type2char(T_BYTE);
  int ret = sprintf(_str, "Y_B_%d", b);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jchar b) {
  _type = type2char(T_CHAR);
  int ret = sprintf(_str, "Y_C_%c", b);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jint i) {
  _type = type2char(T_INT);
  int ret = sprintf(_str, "Y_I_%d", i);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jshort s) {
  _type = type2char(T_SHORT);
  int ret = sprintf(_str, "Y_S_%d", s);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jlong l) {
  _type = type2char(T_LONG);
  int ret = sprintf(_str, "Y_J_%ld", l);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jfloat f) {
  _type = type2char(T_FLOAT);
  int ret = sprintf(_str, "Y_F_%e", f);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jdouble d) {
  _type = type2char(T_DOUBLE);
  int ret = sprintf(_str, "Y_D_%le", d);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

void ConExpression::print() {
  // TODO: include primiteive type
  tty->indent().print("%s", _str);
}

void ConExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) const {
  writer.Key("_type");
  writer.String("ConstExpr");
  writer.Key("_java_type");
  writer.String(_type.c_str());
  writer.Key("_expr");
  writer.String(_str);
}

#endif