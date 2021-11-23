#ifndef SHARE_VM_CONCOLIC_UTILS_HPP
#define SHARE_VM_CONCOLIC_UTILS_HPP

#ifdef ENABLE_CONCOLIC

#define CHARACTER_VALUE_FIELD_INDEX 65
#define BOOLEAN_VALUE_FIELD_INDEX 3
#define BYTE_VALUE_FIELD_INDEX 3
#define INTEGER_VALUE_FIELD_INDEX 7
#define SHORT_VALUE_FIELD_INDEX 3
#define LONG_VALUE_FIELD_INDEX 3
#define FLOAT_VALUE_FIELD_INDEX 11
#define DOUBLE_VALUE_FIELD_INDEX 11

#include "oops/klass.inline.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/handles.hpp"

namespace OopUtils {

bool is_java_string_interned(oop str_obj);

typeArrayOop java_string_to_char_array(oop str_obj);
/**
 * Do not forget to use ResourceMark before this function call!
 */
const char *java_string_to_c(oop str_obj);
jchar java_char_to_c(oop char_obj);
jboolean java_boolean_to_c(oop bool_obj);
jbyte java_byte_to_c(oop byte_obj);
jint java_int_to_c(oop int_obj);
jshort java_short_to_c(oop short_obj);
jlong java_long_to_c(oop long_obj);
jfloat java_float_to_c(oop float_obj);
jdouble java_double_to_c(oop double_obj);
} // namespace OopUtils

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_UTILS_HPP
