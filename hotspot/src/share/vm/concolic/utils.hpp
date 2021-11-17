#ifndef SHARE_VM_CONCOLIC_UTILS_HPP
#define SHARE_VM_CONCOLIC_UTILS_HPP

#include "oops/klass.inline.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/handles.hpp"

namespace OopUtils {

typeArrayOop java_string_to_char_array(oop str_obj);
/**
 * Do not forget to use ResourceMark before this function call!
 */
char *java_string_to_c(oop str_obj);
jint java_int_to_c(oop int_obj);
} // namespace OopUtils

#endif
