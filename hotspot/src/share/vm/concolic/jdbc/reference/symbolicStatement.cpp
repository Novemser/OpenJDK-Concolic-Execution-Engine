
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicStatement.hpp"

const char *SymStmt::TYPE_NAME = "com/mysql/jdbc/StatementImpl";

SymStmt::SymStmt(sym_rid_t sym_rid) : SymInstance(sym_rid) {}

SymStmt::~SymStmt() {}

void SymStmt::print() {}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC