#ifdef ENABLE_CONCOLIC

#include "symbolicPersister.hpp"
#include "symbolicStatement.hpp"

bool SymPersister::is_executing = false;
long SymPersister::execute_counter = 0;

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
  // entity
  set.insert("forceVersionIncrement");
  set.insert("insert");
  set.insert("update");
  set.insert("delete");
  // TODO: collection
  return set;
}

std::map<std::string, std::string> SymPersister::handle_method_signatures = init_handle_method_signatures();

std::map<std::string, std::string> SymPersister::init_handle_method_signatures() {
  std::map<std::string, std::string> map;
  map["insert"] = "(Ljava/io/Serializable;[Ljava/lang/Object;[ZILjava/lang/String;Ljava/lang/Object;Lorg/hibernate/engine/spi/SharedSessionContractImplementor;)V";
  map["update"] = "(Ljava/io/Serializable;[Ljava/lang/Object;[Ljava/lang/Object;Ljava/lang/Object;[ZILjava/lang/Object;Ljava/lang/Object;Ljava/lang/String;Lorg/hibernate/engine/spi/SharedSessionContractImplementor;)Z";
  // TODO: delete, forceVersionIncrement
  return map;
}

std::map<std::string, bool> SymPersister::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymPersister::init_skip_method_names() {
  std::map<std::string, bool> map;
  return map;
}

bool SymPersister::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  NameSignaturePairIt callee_name_signature = handle_method_signatures.find(callee_name);

  bool need_handling = false;
  if (callee_name_signature != handle_method_signatures.end()) {
    const std::string signature = std::string(handle.get_callee_istate()->method()->signature()->as_C_string());
    if (callee_name_signature->second == signature) {
      execute_counter = SymStmt::getExecuteCounter();
      need_handling = true;
      is_executing = true;
      // tty->print_cr(">>>>>>>>>>>>>>>>> got a Persister method call(%d): %s %s %s", need_handling, handle.get_callee_holder_name().c_str(), callee_name.c_str(), signature.c_str());
    }
  }
  return need_handling;
}

Expression *SymPersister::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  if (is_executing && execute_counter + 1 == SymStmt::getExecuteCounter()) {
    // this indicates a SQL is executed, let's record it!
    // tty->print_cr("<<<<<<<<<<<<<<<<< %ld -> %ld", execute_counter, SymStmt::getExecuteCounter());
  } else {
    // tty->print_cr("<<<<<<<<<<<<<<<<< didn't execute %ld -> %ld", execute_counter, SymStmt::getExecuteCounter());
  }
  is_executing = false;
  return NULL;
}

#endif