#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_CONNECTION_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_CONNECTION_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"

class SymConn {
public:
  static const char *TYPE_NAME;
  static std::string sql_template;
  static method_set_t symbolized_methods;

public:
  static bool invoke_method(MethodSymbolizerHandle &handle);
  static void finish_method(MethodSymbolizerHandle &handle);

private:
  static method_set_t init_symbolized_methods();
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_CONNECTION_HPP