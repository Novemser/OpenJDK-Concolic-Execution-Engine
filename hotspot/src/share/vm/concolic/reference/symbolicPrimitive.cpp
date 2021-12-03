#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicPrimitive.hpp"

template <>
const char *SymPrimitive<jchar>::ARRAY_TYPE_NAME = "[Ljava/lang/Character;";
template <>
const char *SymPrimitive<jboolean>::ARRAY_TYPE_NAME = "[Ljava/lang/Boolean;";
template <>
const char *SymPrimitive<jbyte>::ARRAY_TYPE_NAME = "[Ljava/lang/Byte;";
template <>
const char *SymPrimitive<jint>::ARRAY_TYPE_NAME = "[Ljava/lang/Integer;";
template <>
const char *SymPrimitive<jshort>::ARRAY_TYPE_NAME = "[Ljava/lang/Short;";
template <>
const char *SymPrimitive<jlong>::ARRAY_TYPE_NAME = "[Ljava/lang/Long;";
template <>
const char *SymPrimitive<jfloat>::ARRAY_TYPE_NAME = "[Ljava/lang/Float;";
template <>
const char *SymPrimitive<jdouble>::ARRAY_TYPE_NAME = "[Ljava/lang/Double;";

template <> const char *SymPrimitive<jchar>::TYPE_NAME = "java/lang/Character";
template <> const char *SymPrimitive<jboolean>::TYPE_NAME = "java/lang/Boolean";
template <> const char *SymPrimitive<jbyte>::TYPE_NAME = "java/lang/Byte";
template <> const char *SymPrimitive<jint>::TYPE_NAME = "java/lang/Integer";
template <> const char *SymPrimitive<jshort>::TYPE_NAME = "java/lang/Short";
template <> const char *SymPrimitive<jlong>::TYPE_NAME = "java/lang/Long";
template <> const char *SymPrimitive<jfloat>::TYPE_NAME = "java/lang/Float";
template <> const char *SymPrimitive<jdouble>::TYPE_NAME = "java/lang/Double";

template <class T> sym_rid_t SymPrimitive<T>::sym_count = 0;

template <class T>
SymPrimitive<T>::SymPrimitive(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}

template <class T> SymPrimitive<T>::~SymPrimitive() {
  Expression::gc(_exp);
}

template <class T> Expression *SymPrimitive<T>::get(int field_offset) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  return _exp;
}

template <class T>
void SymPrimitive<T>::init_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  exp->inc_ref();
  _exp = exp;
}

template <class T>
void SymPrimitive<T>::set_sym_exp(int field_offset, Expression *exp) {
  Expression::gc(_exp);

  _exp = exp;
  if (_exp) {
    _exp->inc_ref();
  }
}

template <class T> void SymPrimitive<T>::print() {
  tty->print_cr("SymPrimitive: ");
  _exp->print();
  tty->print("\n");
}

template <class T> Expression *SymPrimitive<T>::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    return get_exp_of_helper(obj);
  }
}

template <> Expression *SymPrimitive<jchar>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_char_to_c(obj));
}
template <> Expression *SymPrimitive<jboolean>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_boolean_to_c(obj));
}
template <> Expression *SymPrimitive<jbyte>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_byte_to_c(obj));
}
template <> Expression *SymPrimitive<jint>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_int_to_c(obj));
}
template <> Expression *SymPrimitive<jshort>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_short_to_c(obj));
}
template <> Expression *SymPrimitive<jlong>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_long_to_c(obj));
}
template <> Expression *SymPrimitive<jfloat>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_float_to_c(obj));
}
template <> Expression *SymPrimitive<jdouble>::get_exp_of_helper(oop obj) {
  return new ConExpression(OopUtils::java_double_to_c(obj));
}

template class SymPrimitive<jchar>;
template class SymPrimitive<jboolean>;
template class SymPrimitive<jbyte>;
template class SymPrimitive<jint>;
template class SymPrimitive<jshort>;
template class SymPrimitive<jlong>;
template class SymPrimitive<jfloat>;
template class SymPrimitive<jdouble>;

#endif
