
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/SymbolicOp.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/utils.hpp"

std::set<std::string> SymResSet::target_class_names = init_target_class_names();

std::set<std::string> SymResSet::init_target_class_names() {
  std::set<std::string> set;
  set.insert("com/mysql/jdbc/JDBC42ResultSet");
  set.insert("com/mysql/jdbc/ResultSetImpl");
  return set;
}

std::set<std::string> SymResSet::skip_method_names = init_skip_method_names();

std::set<std::string> SymResSet::init_skip_method_names() {
  std::set<std::string> set;
  set.insert("close");
  set.insert("realClose");
  return set;
}

SymResSet::SymResSet(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _sym_stmt_rid(0), _sql_id(0), _row_id(0) {}

SymResSet::~SymResSet() {
  if (_size_exp->dec_ref()) {
    delete _size_exp;
  }
}

void SymResSet::print() {
  tty->print_cr("SymResSet of SymStmt %lu", _sym_stmt_rid);
}

bool SymResSet::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;

  if (callee_name == "next") {
    oop res_set_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(res_set_obj);
    sym_res_set->next();
    need_symbolize = true;
  } else if (skip_method_names.find(callee_name) != skip_method_names.end()) {
    need_symbolize = true;
  } else {
    tty->print_cr("%s: %s", handle.get_callee_holder_name().c_str(),
                  handle.get_callee_name().c_str());
    // ShouldNotCallThis();
  }

  return need_symbolize;
}

Expression *SymResSet::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;
  if (strncmp("get", callee_name.c_str(), 3) == 0) {
    oop this_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(this_obj);

    oop col_str_obj = handle.get_param<oop>(1);
    assert(col_str_obj->klass()->name()->equals(SymString::TYPE_NAME),
           "should be");

    ResourceMark rm;
    const char *col_name = OopUtils::java_string_to_c(col_str_obj);

    BasicType type = handle.get_result_type();
    switch (type) {
    case T_VOID:
    case T_OBJECT:
    case T_ARRAY:
      ShouldNotCallThis();
      break;
    default:
      exp = new ResultSetSymbolExp(sym_res_set, col_name);
    }
  } else if (callee_name == "next") {
    oop this_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(this_obj);
    jboolean ret = handle.get_result<jboolean>(T_BOOLEAN);

    ConcolicMngr::record_path_condition(new OpSymExpression(
        sym_res_set->get_size_exp(),
        new ConExpression(sym_res_set->get_row_id()), op_ge, ret));
  }

  return exp;
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set) {
  int length = sprintf(str_buf, "RS_%d.size", sym_res_set->_sql_id);
  set(str_buf, length);
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set,
                                       const char *col_name) {
  int length = sprintf(str_buf, "RS_%d_%d_%s", sym_res_set->_sql_id,
                       sym_res_set->_row_id, col_name);
  set(str_buf, length);
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC