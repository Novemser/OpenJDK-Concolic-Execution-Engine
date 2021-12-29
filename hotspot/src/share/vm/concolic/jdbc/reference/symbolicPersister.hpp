#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPERSISTER_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPERSISTER_HPP

#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizerHandle.hpp"

#ifdef ENABLE_CONCOLIC

class SymPersister {
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

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static bool check_param_symbolized(MethodSymbolizerHandle &handle);
  static int check_param_symbolized_helper(MethodSymbolizerHandle &handle, BasicType type,
                                           int locals_offset, bool &recording);

  static void prepare_param(MethodSymbolizerHandle &handle);
  static int prepare_param_helper(MethodSymbolizerHandle &handle, BasicType type,
                                  int locals_offset);
};


#endif // ENABLE_CONCOLIC
#endif //SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPERSISTER_HPP
