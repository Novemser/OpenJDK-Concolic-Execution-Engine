#ifdef ENABLE_CONCOLIC

#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/reference/symbolicInteger.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

ArrayInitExpression::ArrayInitExpression(sym_rid_t array_id, arrayOop array) {
  int ret = sprintf(_arr_str, "A_%lu_init", array_id);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");

  ArrayKlass *ak = (ArrayKlass *)array->klass();
  BasicType T = ak->element_type();

  for (int i = 0; i < array->length(); i++) {
    Expression *value_exp = NULL;
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
    default: {
      assert(T == T_OBJECT, "should be");
      if (ak->name()->equals(SymInteger::ARRAY_TYPE_NAME)) {
        value_exp = SymInteger::get_exp_of(
            *(oop *)(((address)array->base(T)) + i * sizeof(oop)));
      }
    }
    /**
     * TODO: add AALOAD && AASTORE support.
     */
    break;
    }
    if (value_exp) {
      value_exp->inc_ref();
    }
    _arr_exps.push_back(value_exp);
  }
}

ArrayInitExpression::~ArrayInitExpression() {
  int size = _arr_exps.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _arr_exps[i];
    if (exp && exp->dec_ref()) {
      delete exp;
    }
  }
}

void ArrayInitExpression::print() {
  tty->print("%s{", _arr_str);
  int size = _arr_exps.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _arr_exps[i];
    if (exp) {
      exp->print();
    } else {
      tty->print("NULL");
    }
    if (i < size - 1) {
      tty->print(",");
    }
  }
  tty->print("}");
}

#endif