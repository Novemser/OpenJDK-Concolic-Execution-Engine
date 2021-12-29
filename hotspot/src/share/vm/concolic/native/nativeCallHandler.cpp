#ifdef ENABLE_CONCOLIC

#include "nativeCallHandler.hpp"
#include "nativeGetPrimitiveHandler.hpp"

void NativeCallHandler::handle_native(Method *method, intptr_t *locals) {
  ResourceMark rm;
  const std::string name_sig(method->name_and_sig_as_C_string());
  BasicType result_type = method->result_type();
  if (is_java_primitive(result_type)) {
    if (NativeGetPrimitiveHandler::target(name_sig)) {
      NativeGetPrimitiveHandler::handle_native(locals, result_type);
    }
  }
}


#endif