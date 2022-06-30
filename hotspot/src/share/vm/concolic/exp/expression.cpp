#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "concolic/concolicMngr.hpp"
#include "webridge/utils/rapidjson/document.h"
#include "utilities/ostream.hpp"
#include <cstring>

ulong Expression::total_count = 0;
ulong Expression::unique_id = 0;
std::set<Expression *> Expression::deleted;
std::set<Expression *> Expression::keep;

Expression::Expression() : _ref_count(0), _unique_id(unique_id++) {
  keep.insert(this);
  if (deleted.count(this) > 0) {
    deleted.erase(this);
  }
  total_count++;
}

Expression::~Expression() {
  deleted.insert(this);
  total_count--;
}

void Expression::gc(Expression *exp) {
  if (exp && deleted.count(exp) > 0) {
    return;
  }

  if (exp && exp->dec_ref()) {
    delete exp;
  }
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


void Expression::init_gc_helper() {
  // just perform sanity check
  guarantee(deleted.empty(), "should be empty, did u forget to call endConcolic()?");
  guarantee(keep.empty(), "should be empty, did u forget to call endConcolic()?");
}

void Expression::finalize_dangling_objects() {
  tty->print_cr("Cleaning %lu dangling expressions", keep.size());
  for (std::set<Expression *>::iterator iter = keep.begin(); iter != keep.end(); ++iter) {
    Expression* exp = *iter;
    if (deleted.count(exp) > 0) {
      continue;
    }
    delete exp;
  }
  guarantee(total_count == 0, "total_count should be 0");
  keep.clear();
  deleted.clear();
}

std::string Expression::get_name() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);
  serialize(writer);
  rapidjson::Document doc;
  doc.Parse(s.GetString());
  if (doc.HasMember("_exp")) {
    return doc["_exp"].GetString();
  } else {
    guarantee(false, (std::string("Not implemented expression get_name:") + s.GetString()).c_str());
  }
}

OpSymExpression::OpSymExpression(Expression *l, Expression *r, SymbolicOp op,
                                 bool cmp)
    : _left(l), _right(r), _op(cmp ? op : NotSymbolicOp[op]), _left_cache(NULL), _right_cache(NULL) {
  if (_left) {
    _left->inc_ref();
  }
  if (_right) {
    _right->inc_ref();
  }
  // tty->print_cr("Create: %p -> %p, %p", this, _left, _right);
}

OpSymExpression::OpSymExpression(Expression *r, SymbolicOp op)
    : _left(NULL), _right(r), _op(op), _left_cache(NULL), _right_cache(NULL) {
  if (_right) {
    _right->inc_ref();
  }
  // tty->print_cr("Create: %p -> %p, %p", this, _left, _right);
}

OpSymExpression::~OpSymExpression() {
  Expression::gc(_left);
  Expression::gc(_right);
  delete[] _left_cache;
  delete[] _right_cache;
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

void OpSymExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  using namespace rapidjson;
  writer.Key("_type");
  writer.String("BinaryExpression");

  writer.Key("_left");
  if (_left) {
//    _left->serialize(writer);
    if (!_left_cache) {
      StringBuffer s;
      Writer<StringBuffer> cache_writer(s);
      _left->serialize(cache_writer);
      _left_cache = new char[s.GetLength() + 1];
      strcpy(_left_cache, s.GetString());
    }
    writer.RawValue(_left_cache, strlen(_left_cache), kObjectType);
  } else {
    writer.Null();
  }

  writer.Key("_op");
  writer.String(SymbolicOpStr[(int) _op]);

  writer.Key("_right");
  if (_right) {
//    _right->serialize(writer);
    if (!_right_cache) {
      StringBuffer s;
      Writer<StringBuffer> cache_writer(s);
      _right->serialize(cache_writer);
      _right_cache = new char[s.GetLength() + 1];
      strcpy(_right_cache, s.GetString());
    }
    writer.RawValue(_right_cache, strlen(_right_cache), kObjectType);
  } else {
    writer.Null();
  }
}

std::string OpSymExpression::get_name() {
  std::string name = "";
  if (_left) {
    name = _left->get_name();
  } else {
    name = "null";
  }
  name = name + SymbolicOpStr[_op];
  if (_right) {
    name = name + _right->get_name();
  } else {
    name = name + "null";
  }
  return name;
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

void ConExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("ConstExpr");
  writer.Key("_java_type");
  writer.String(_type.c_str());
  writer.Key("_exp");
  writer.String(_str);
}

#endif