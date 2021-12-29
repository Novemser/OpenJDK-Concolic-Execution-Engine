#ifdef ENABLE_CONCOLIC

#include "symbolicPersister.hpp"

std::set<std::string> SymPersister::target_class_names = init_target_class_names();

std::set<std::string> SymPersister::init_target_class_names() {
  std::set<std::string> set;
  // entity
  set.insert("org/hibernate/persister/entity/AbstractEntityPersister");
  set.insert("org/hibernate/persister/entity/SingleTableEntityPersister");
  set.insert("org/hibernate/persister/entity/JoinedSubclassEntityPersister");
  set.insert("org/hibernate/persister/entity/UnionSubclassEntityPersister");
  // TODO: collection
  set.insert("org/hibernate/persister/collection/BasicCollectionPersister");
  set.insert("org/hibernate/persister/collection/AbstractCollectionPersister");
  set.insert("org/hibernate/persister/collection/OneToManyPersister");
  return set;
}

std::set<std::string> SymPersister::handle_method_names = init_handle_method_names();

std::set<std::string> SymPersister::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("forceVersionIncrement");
  set.insert("insert");
  set.insert("update");
  set.insert("delete");
  return set;
}

std::map<std::string, bool> SymPersister::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymPersister::init_skip_method_names() {
  std::map<std::string, bool> map;
  return map;
}

bool SymPersister::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    tty->print_cr(">>>>>>>>>>>>>>>>> got a method call: %s %s", handle.get_callee_holder_name().c_str(), callee_name.c_str());
  } else {
    tty->print_cr(">>>>>>>>>>>>>>> not included method: %s %s", handle.get_callee_holder_name().c_str(), callee_name.c_str());
  }
  return false;
}

Expression *SymPersister::finish_method_helper(MethodSymbolizerHandle &handle) {
  return NULL;
}

bool SymPersister::check_param_symbolized(MethodSymbolizerHandle &handle) {
  return false;
}

int SymPersister::check_param_symbolized_helper(MethodSymbolizerHandle &handle, BasicType type, int locals_offset,
                                                     bool &recording) {
  return 0;
}

void SymPersister::prepare_param(MethodSymbolizerHandle &handle) {

}

int SymPersister::prepare_param_helper(MethodSymbolizerHandle &handle, BasicType type, int locals_offset) {
  return 0;
}

#endif