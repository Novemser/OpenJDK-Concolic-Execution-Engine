#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "symbolicHibernateMethod.hpp"
#include "concolic/methodSymbolizer.hpp"

std::set<std::string> SymHibernateMethod::target_class_names = init_target_class_names();

std::set<std::string> SymHibernateMethod::init_target_class_names() {
  std::set<std::string> set;
  set.insert("org/hibernate/id/enhanced/TableGenerator");
  return set;
}

std::set<std::string> SymHibernateMethod::handle_method_names = init_handle_method_names();

std::set<std::string> SymHibernateMethod::init_handle_method_names() {
  std::set<std::string> m_set;
  m_set.insert("generate");
  return m_set;
}

void SymHibernateMethod::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}

bool SymHibernateMethod::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    MethodSymbolizer::invoke_method_helper(handle);
    return true;
  }
  return false;
}

Expression* SymHibernateMethod::finish_method_helper(MethodSymbolizerHandle &handle) {
  return MethodSymbolizer::finish_method_helper(handle);
}



#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
