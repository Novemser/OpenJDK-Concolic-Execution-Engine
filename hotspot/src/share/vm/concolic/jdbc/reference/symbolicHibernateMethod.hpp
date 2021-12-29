#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_HIBERNATE_METHOD_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_HIBERNATE_METHOD_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"


class SymHibernateMethod {
public:
  static bool target(const std::string& name_sig);

private:
  static std::set<std::string> handle_method_name_sigs;

  static std::set<std::string> init_handle_method_name_sigs();

};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif //SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_HIBERNATE_METHOD_HPP
