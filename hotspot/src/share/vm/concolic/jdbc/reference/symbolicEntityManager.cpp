#ifdef ENABLE_CONCOLIC

#include "symbolicEntityManager.hpp"

std::set<std::string> SymEntityManager::target_class_names = init_target_class_names();

std::set<std::string> SymEntityManager::init_target_class_names() {
  std::set<std::string> set;
  // note that we intercept methods of `SessionImpl` because the `em` is proxy of `SessionImpl` in hibernate
  set.insert("org/hibernate/internal/SessionImpl");
  return set;
}

std::map<std::string, std::string> SymEntityManager::handle_method_signatures = init_handle_method_signatures();

std::map<std::string, std::string> SymEntityManager::init_handle_method_signatures() {
  std::map<std::string, std::string> map;
  map["merge"] = "";
  map["persist"] = "";
  map["remove"] = "";
  map["find"] = "";
  map["flush"] = "";
  map["refresh"] = "";
  // TODO: many others
  return map;
}

std::map<std::string, bool> SymEntityManager::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymEntityManager::init_skip_method_names() {
  std::map<std::string, bool> map;
  return map;
}

bool SymEntityManager::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  NameSignaturePairIt callee_name_signature = handle_method_signatures.find(callee_name);

  bool need_handling = false;
  if (callee_name_signature != handle_method_signatures.end()) {
    const std::string signature = std::string(handle.get_callee_istate()->method()->signature()->as_C_string());
    tty->print_cr(">>>>>> got a SessionImpl method call(%d): %s %s %s", need_handling, handle.get_callee_holder_name().c_str(), callee_name.c_str(), signature.c_str());
  }
  return need_handling;
}

Expression *SymEntityManager::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  return NULL;
}

#endif
