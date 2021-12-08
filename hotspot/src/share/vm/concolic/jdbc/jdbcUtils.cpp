#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/jdbcUtils.hpp"

jlong JdbcUtils::get_conn_connection_id(oop obj) {
  Klass *klass = obj->klass();
  if (klass->name()->equals("com/mysql/jdbc/JDBC4Connection")) {
    return obj->long_field(1736);
  } else {
    tty->print_cr("Unhandled JDBC connection class");
    tty->print_cr(klass->name()->as_C_string());
  }
}

jlong JdbcUtils::get_stmt_connection_id(oop obj) {
  Klass *klass = obj->klass();
  if (SymStmt::target(std::string(klass->name()->as_C_string()))) {
    return obj->long_field(24);
  } else {
    tty->print_cr("Unhandled JDBC statement class");
    tty->print_cr(klass->name()->as_C_string());
  }
}

#endif