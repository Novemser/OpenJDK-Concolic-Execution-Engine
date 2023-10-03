#ifdef ENABLE_CONCOLIC

#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/reference/symbolicPrimitive.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

ArrayInitExpression::ArrayInitExpression(sym_rid_t array_id, arrayOop array) {
  ArrayKlass *ak = (ArrayKlass *)array->klass();
  BasicType T = ak->element_type();

  int ret = 0;
  if (array_id == NULL_SYM_RID) {
    ret = sprintf(_arr_str, "A%c", type2char(T));
  } else {
    ret = sprintf(_arr_str, "A%c_%lu", type2char(T), array_id);
  }
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");

  for (int i = 0; i < array->length(); i++) {
    Expression *value_exp = NULL;
    switch (T) {
    case T_BOOLEAN:
      value_exp = new ConExpression(
          *(jboolean *)(((address)array->base(T)) + i * sizeof(jboolean)));
      break;
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
      if (ak->name()->equals(SymPrimitive<jchar>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jchar>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jboolean>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jboolean>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jbyte>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jbyte>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jint>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jint>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jshort>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jshort>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jlong>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jlong>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jfloat>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jfloat>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymPrimitive<jdouble>::ARRAY_TYPE_NAME)) {
        value_exp = SymPrimitive<jdouble>::get_exp_of(get_obj(array, T, i));
      } else if (ak->name()->equals(SymString::ARRAY_TYPE_NAME)) {
        value_exp = SymString::get_exp_of(get_obj(array, T, i));
      } else {
        ResourceMark rm;
        tty->print_cr("Not implemented object array load for class:%s",
                      ak->name()->as_C_string());
        ShouldNotReachHere();
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
    Expression::gc(_arr_exps[i]);
  }
}

void ArrayInitExpression::print() {
  tty->print("(array_init %s", _arr_str);
  int size = _arr_exps.size();
  for (int i = 0; i < size; ++i) {
    Expression *exp = _arr_exps[i];
    tty->print(" ");
    if (exp) {
      exp->print();
    } else {
      tty->print("NULL");
    }
  }
  tty->print(")");
}

oop ArrayInitExpression::get_obj(arrayOop array, BasicType T, int i) {
  return *(oop *)(((address)array->base(T)) + i * sizeof(oop));
}

void ArrayInitExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("array_init_expr");
  writer.Key("_arr_str");
  writer.String(_arr_str);
  writer.Key("_arr_exps");
  writer.StartArray();
  for (size_t index = 0; index < _arr_exps.size(); ++index) {
    Expression* expr = _arr_exps[index];
    if (expr) {
      expr->serialize(writer);
    } else {
      writer.Null();
    }
  }
  writer.EndArray();
}

std::string ArrayInitExpression::get_name() {
  std::string name = "";
  name += _arr_str;
  for (size_t index = 0; index < _arr_exps.size(); ++index) {
    Expression* expr = _arr_exps[index];
    if (expr) {
      name += expr->get_name();
    } else {
      name += "null";
    }

    if (index != (_arr_exps.size() - 1)) {
      name += "_";
    }
  }
  return name;
}

#endif