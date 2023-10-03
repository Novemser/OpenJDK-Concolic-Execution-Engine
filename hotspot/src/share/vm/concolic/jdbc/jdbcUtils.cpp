#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/jdbcUtils.hpp"

jlong JdbcUtils::get_conn_connection_id(oop obj) {
  Klass *klass = obj->klass();
  if (klass->name()->equals("com/mysql/jdbc/JDBC4Connection")) {
    return obj->long_field(1736);
  } else {
    ResourceMark rm;
    tty->print_cr("Unhandled JDBC connection class: %s", klass->name()->as_C_string());
    ShouldNotCallThis();
  }
}

jlong JdbcUtils::get_stmt_connection_id(oop obj) {
  Klass *klass = obj->klass();
  ResourceMark rm;
  std::string class_name(klass->name()->as_C_string());
  if (SymStmt::target(class_name)) {
    return obj->long_field(24);
  } else {
    tty->print_cr("Unhandled JDBC statement class: %s", klass->name()->as_C_string());
    ShouldNotCallThis();
  }
}

#endif