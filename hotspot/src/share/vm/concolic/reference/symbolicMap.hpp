#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICMAP_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICMAP_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"

class SymMap {
public:
  inline static bool target(const std::string &class_name) {
    return target_class_names.find(class_name) != target_class_names.end();
  }

private:
  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> skip_method_names;
  static std::set<std::string> init_skip_method_names();
  static bool need_recording;

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static int prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                           intptr_t *locals, int locals_offset,
                           bool &recording);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICMAP_HPP