#ifdef ENABLE_CONCOLIC

#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/instance/symbolicArray.hpp"
#include "utilities/ostream.hpp"

ArrayInitExpression::ArrayInitExpression(sym_oid_t array_id, arrayOop array) {
  int ret = sprintf(_arr_str, "A_%lu_init", array_id);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");

  ArrayKlass *ak = (ArrayKlass *)array->klass();
  BasicType T = ak->element_type();
  Expression *value_exp;

  for (int i = 0; i < array->length(); i++) {
    switch (T) {
    case T_INT:
      value_exp = new ConExpression(
          *(jint *)(((address)array->base(T)) + i * sizeof(jint)));
      break;
    case T_FLOAT:
      value_exp = new ConExpression(
          *(jfloat *)(((address)array->base(T)) + i * sizeof(jfloat)));
      break;
    case T_BYTE:
      value_exp = new ConExpression(
          *(jbyte *)(((address)array->base(T)) + i * sizeof(jbyte)));
      break;
    case T_CHAR:
      value_exp = new ConExpression(
          *(jchar *)(((address)array->base(T)) + i * sizeof(jchar)));
      break;
    case T_SHORT:
      value_exp = new ConExpression(
          *(jshort *)(((address)array->base(T)) + i * sizeof(jshort)));
      break;
    case T_LONG:
      value_exp = new ConExpression(
          *(jlong *)(((address)array->base(T)) + i * sizeof(jlong)));
      break;
    case T_DOUBLE:
      value_exp = new ConExpression(
          *(jdouble *)(((address)array->base(T)) + i * sizeof(jdouble)));
      break;
    default:
      break;
    }

    value_exp->inc_ref();
    _arr_exps.push_back(value_exp);
  }
}

ArrayInitExpression::~ArrayInitExpression() {
  int size = _arr_exps.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _arr_exps[i];
    if (exp->dec_ref()) {
      delete exp;
    }
  }
}

void ArrayInitExpression::print() {
  tty->print("%s{", _arr_str);
  int size = _arr_exps.size();
  for (int i = 0; i < size; ++i) {
    _arr_exps[i]->print();
    if (i < size - 1) {
      tty->print(",");
    }
  }
  tty->print("}");
}

#endif