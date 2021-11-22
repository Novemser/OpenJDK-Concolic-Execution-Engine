
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"

const char *SymResSet::TYPE_NAME = "com/mysql/jdbc/JDBC42ResultSet";
const char *SymResSet::BASE_TYPE_NAME = "com/mysql/jdbc/ResultSet";

SymResSet::SymResSet(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _sym_stmt_rid(0) {}

SymResSet::~SymResSet() {}

void SymResSet::print() {
  tty->print_cr("SymResSet of SymStmt %lu", _sym_stmt_rid);
}

bool SymResSet::invoke_method(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;

  if (callee_name == "setInt") {

    need_symbolize = true;
  } else {
    ShouldNotCallThis();
  }

  return need_symbolize;
}

void SymResSet::finish_method(MethodSymbolizerHandle &handle) {}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC