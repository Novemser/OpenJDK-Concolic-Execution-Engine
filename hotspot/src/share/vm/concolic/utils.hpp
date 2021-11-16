#ifndef SHARE_VM_CONCOLIC_UTILS_HPP
#define SHARE_VM_CONCOLIC_UTILS_HPP

#include "oops/oop.inline.hpp"
#include "oops/klass.inline.hpp"
#include "runtime/handles.hpp"


namespace OopUtils {

typeArrayOop charArrayObjFromJavaString(oop str_obj);
char* javaStringToC(oop str_obj);

} // namespace ConcolicUtils

#endif
