#ifdef ENABLE_CONCOLIC

#include "concolic/utils.hpp"
#include "classfile/symbolTable.hpp"
#include "concolic/fieldTraverser.hpp"
#include "concolic/reference/symbolicPrimitive.hpp"
#include "concolic/reference/symbolicString.hpp"

bool OopUtils::is_java_string_interned(oop str_obj) {
  ResourceMark rm;

  typeArrayOop char_array_obj = java_string_to_char_array(str_obj);
  int length = char_array_obj->length();
  jchar *char_array = (jchar *)char_array_obj->base(T_CHAR);

  oop str_in_table = StringTable::lookup(char_array, length);
  return str_obj == str_in_table;
}

typeArrayOop OopUtils::java_string_to_char_array(oop str_obj) {
  Klass *klass = str_obj->klass();
  assert(klass->name()->equals(SymString::TYPE_NAME), "should be String");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = 0;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  typeArrayOop char_array_obj =
      (typeArrayOop)(str_obj->obj_field(field_offset));
  return char_array_obj;
}

const char *OopUtils::java_string_to_c(oop str_obj) {
  typeArrayOop char_array_obj = java_string_to_char_array(str_obj);
  if (char_array_obj) {
    int length = char_array_obj->length();
    char *char_array = NEW_RESOURCE_ARRAY(char, length + 1);
    for (int i = 0; i < length; i++) {
      jchar jc = char_array_obj->char_at(i);
      assert(0 <= jc && jc <= 0xff, "only deal with single-byte char now");
      char c = *(char *)&jc;
      char_array[i] = c;
    }
    char_array[length] = 0;

    return char_array;
  } else {
    return "";
  }
}
jchar OopUtils::java_char_to_c(oop char_obj) {
  Klass *klass = char_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jchar>::TYPE_NAME),
         "should be Character");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = CHARACTER_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return char_obj->char_field(field_offset);
}

jboolean OopUtils::java_boolean_to_c(oop bool_obj) {
  Klass *klass = bool_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jboolean>::TYPE_NAME),
         "should be Boolean");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = BOOLEAN_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return bool_obj->bool_field(field_offset);
}

jbyte OopUtils::java_byte_to_c(oop byte_obj) {
  Klass *klass = byte_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jbyte>::TYPE_NAME),
         "should be Byte");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = BYTE_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return byte_obj->byte_field(field_offset);
}

jint OopUtils::java_int_to_c(oop int_obj) {
  Klass *klass = int_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jint>::TYPE_NAME),
         "should be Integer");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = INTEGER_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return int_obj->int_field(field_offset);
}

jshort OopUtils::java_short_to_c(oop short_obj) {
  Klass *klass = short_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jshort>::TYPE_NAME),
         "should be Short");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = SHORT_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return short_obj->short_field(field_offset);
}

jlong OopUtils::java_long_to_c(oop long_obj) {
  Klass *klass = long_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jlong>::TYPE_NAME),
         "should be Long");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = LONG_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return long_obj->long_field(field_offset);
}

jfloat OopUtils::java_float_to_c(oop float_obj) {
  Klass *klass = float_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jfloat>::TYPE_NAME),
         "should be Float");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = FLOAT_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return float_obj->float_field(field_offset);
}

jdouble OopUtils::java_double_to_c(oop double_obj) {
  Klass *klass = double_obj->klass();
  assert(klass->name()->equals(SymPrimitive<jdouble>::TYPE_NAME),
         "should be Double");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = DOUBLE_VALUE_FIELD_INDEX;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return double_obj->double_field(field_offset);
}

#endif