#ifdef ENABLE_CONCOLIC

#include "concolic/utils.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/reference/symbolicInteger.hpp"
#include "concolic/fieldTraverser.hpp"

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
}

char *OopUtils::java_string_to_c(oop str_obj) {
  typeArrayOop char_array_obj = java_string_to_char_array(str_obj);

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
}

jint OopUtils::java_int_to_c(oop int_obj) {
  Klass *klass = int_obj->klass();
  assert(klass->name()->equals(SymInteger::TYPE_NAME), "should be Integer");
  InstanceKlass *ik = (InstanceKlass *)klass;

  const int value_field_index = 7;
  assert(ik->field_name(value_field_index)->equals("value"),
         "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  return int_obj->int_field(field_offset);
}

#endif