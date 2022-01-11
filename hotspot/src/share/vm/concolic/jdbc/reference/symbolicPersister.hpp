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
  typedef std::map<std::string, std::string>::iterator NameSignaturePairIt;

  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> handle_method_names;
  static std::set<std::string> init_handle_method_names();
  static std::map<std::string, std::string> handle_method_signatures;
  static std::map<std::string, std::string> init_handle_method_signatures();
  static std::map<std::string, bool> skip_method_names;
  static std::map<std::string, bool> init_skip_method_names();

  // indicating is executing something like insert, update, delete
  static bool is_executing;
  static long execute_counter;

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
};


#endif // ENABLE_CONCOLIC
#endif //SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICPERSISTER_HPP
