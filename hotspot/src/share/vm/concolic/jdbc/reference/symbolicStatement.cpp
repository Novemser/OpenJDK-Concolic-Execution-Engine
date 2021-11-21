
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicStatement.hpp"

// const char *SymStmt::TYPE_NAME = "com/mysql/jdbc/StatementImpl";
const char *SymStmt::TYPE_NAME = "com/mysql/jdbc/JDBC42PreparedStatement";

SymStmt::SymStmt(sym_rid_t sym_rid) : SymInstance(sym_rid), _sql_template("") {}

SymStmt::~SymStmt() {}

void SymStmt::print() { tty->print_cr("SymStmt: %s", _sql_template.c_str()); }

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC