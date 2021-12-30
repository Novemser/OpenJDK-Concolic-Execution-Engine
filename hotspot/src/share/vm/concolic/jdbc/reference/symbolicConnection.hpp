#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_CONNECTION_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_CONNECTION_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymConn {
public:
  inline static bool target(const std::string &class_name) {
    return class_name == TYPE_NAME;
  }
private:
  static const char *TYPE_NAME;
  static method_set_t init_handle_method_names();
  static method_set_t handle_method_names;
  static method_set_t skip_method_names;
  static method_set_t init_skip_method_names();

public:
  static std::string sql_template;

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void init_register_class(MethodSymbolizer *m_symbolizer);

};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_CONNECTION_HPP