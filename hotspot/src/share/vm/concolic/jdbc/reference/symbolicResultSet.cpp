
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"

const char *SymResSet::TYPE_NAME = "com/mysql/jdbc/JDBC42ResultSet";
const char *SymResSet::BASE_TYPE_NAME = "com/mysql/jdbc/ResultSetImpl";

SymResSet::SymResSet(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _sym_stmt_rid(0), _row_id(-1) {}

SymResSet::~SymResSet() {}

void SymResSet::print() {
  tty->print_cr("SymResSet of SymStmt %lu", _sym_stmt_rid);
}

bool SymResSet::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  // bool need_symbolize = true;

  if (callee_name == "next") {
    oop res_set_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(res_set_obj);
    sym_res_set->next();
  }

  return true;
}

Expression *SymResSet::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;
  if (strncmp("get", callee_name.c_str(), 3) == 0) {
    BasicType type = handle.get_result_type();

    switch (type) {
    case T_VOID:
    case T_OBJECT:
    case T_ARRAY:
      ShouldNotCallThis();
      break;
    default:
      exp = new SymbolExpression();
    }
  }

  return exp;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC