#ifndef SHARE_VM_NATIVE_GETPTIMITIVE_HANDLE_HPP
#define SHARE_VM_NATIVE_GETPTIMITIVE_HANDLE_HPP

#ifdef ENABLE_CONCOLIC

#include "oops/oop.inline.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/concolicMngr.hpp"
#include <set>
#include <string>

class NativeGetPrimitiveHandler {
public:
  static bool target(const std::string& name_sig);

  static void handle_native(intptr_t* locals, BasicType result_type);

private:
  static std::set<std::string> handle_method_name_sigs;

  static std::set<std::string> init_handle_method_name_sigs();
};


#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_NATIVE_GETPTIMITIVE_HANDLE_HPP
