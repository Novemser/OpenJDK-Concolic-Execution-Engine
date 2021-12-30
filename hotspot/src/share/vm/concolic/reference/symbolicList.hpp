#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICLIST_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICLIST_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymList {
public:
  inline static bool target(const std::string &class_name) {
    return target_class_names.find(class_name) != target_class_names.end();
  }

private:
  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> handle_method_names;
  static std::set<std::string> init_handle_method_names();
  static std::map<std::string, bool> skip_method_names;
  static std::map<std::string, bool> init_skip_method_names();
  static bool need_recording;

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void init_register_class(MethodSymbolizer *m_symbolizer);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICLIST_HPP