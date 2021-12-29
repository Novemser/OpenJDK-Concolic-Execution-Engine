#ifndef SHARE_VM_NATIVECALL_HANDLER_HPP
#define SHARE_VM_NATIVECALL_HANDLER_HPP

#ifdef ENABLE_CONCOLIC

#include "oops/oop.inline.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/concolicMngr.hpp"
#include <string>

class NativeCallHandler {
public:
  static void handle_native(Method *method, intptr_t* locals);
};

#endif // ENABLE_CONCOLIC
#endif //SHARE_VM_NATIVECALL_HANDLER_HPP
