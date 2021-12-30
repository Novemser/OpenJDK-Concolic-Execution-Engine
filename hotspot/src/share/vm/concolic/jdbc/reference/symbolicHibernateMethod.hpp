#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_HIBERNATE_METHOD_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_HIBERNATE_METHOD_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/methodSymbolizer.hpp"


class SymHibernateMethod {
public:
  static std::set<std::string> target_class_names;
  static std::set<std::string> handle_method_names;

  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static void init_register_class(MethodSymbolizer *m_symbolizer);

private:
  static std::set<std::string> handle_method_name_sigs;

  static std::set<std::string> init_target_class_names();
  static std::set<std::string> init_handle_method_names();

};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif //SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_HIBERNATE_METHOD_HPP
