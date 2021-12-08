#ifndef SHARE_VM_CONCOLIC_JDBC_UTILS_HPP
#define SHARE_VM_CONCOLIC_JDBC_UTILS_HPP
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "oops/klass.inline.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/handles.hpp"
#include "reference/symbolicStatement.hpp"

#include <string>

namespace JdbcUtils {
  jlong get_conn_connection_id(oop obj);

  jlong get_stmt_connection_id(oop obj);
}


#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif //SHARE_VM_CONCOLIC_JDBC_UTILS_HPP
