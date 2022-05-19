#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRINGUTILS_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRINGUTILS_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymStrUtils {
public:
  static std::set<std::string> target_class_names;
  static method_set_t handle_method_names;
  static std::map<std::string, bool> skip_method_names;
  static bool need_recording;

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void prepare_param(MethodSymbolizerHandle &handle);
  static int prepare_param_helper(MethodSymbolizerHandle &handle, BasicType type,
                                  int locals_offset);

  static void init_register_class(MethodSymbolizer* m_symbolizer);
  static std::set<std::string> init_target_class_names();

private:
  static method_set_t init_handle_method_names();
  static std::map<std::string, bool> init_skip_method_names();
};


#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRINGUTILS_HPP