
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/utils.hpp"

std::set<std::string> SymStmt::target_class_names = init_target_class_names();

std::set<std::string> SymStmt::init_target_class_names() {
  std::set<std::string> set;
  set.insert("com/mysql/jdbc/JDBC42PreparedStatement");
  set.insert("com/mysql/jdbc/PreparedStatement");
  set.insert("com/mysql/jdbc/StatementImpl");
  return set;
}

std::set<std::string> SymStmt::skip_method_names = init_skip_method_names();

std::set<std::string> SymStmt::init_skip_method_names() {
  std::set<std::string> set;
  set.insert("setInt");
  set.insert("close");
  set.insert("checkClosed");
  set.insert("<init>");
  set.insert("isEscapeNeededForString");
  set.insert("setInternal");
  set.insert("setResultSetType");
  set.insert("setResultSetConcurrency");
  set.insert("getParameterIndexOffset");
  set.insert("getMaxRows");
  set.insert("setMaxRows");
  set.insert("getQueryTimeout");
  set.insert("setQueryTimeout");
  return set;
}

SymStmt::SymStmt(sym_rid_t sym_rid) : SymInstance(sym_rid), _sql_template("") {}

SymStmt::~SymStmt() {
  exp_map_t::iterator iter;
  for (iter = _param_exps.begin(); iter != _param_exps.end(); ++iter) {
    Expression *exp = iter->second;
    if (exp && exp->dec_ref()) {
      delete exp;
    }
  }
}

void SymStmt::set_param(int index, Expression *exp) {
  exp_map_t::iterator iter = _param_exps.find(index);
  assert(iter == _param_exps.end(),
         "currently, assume each parameter is only set once");
  exp->inc_ref();
  _param_exps.insert(std::make_pair(index, exp));
}

void SymStmt::print() {
  tty->print_cr("SymStmt: %s", _sql_template.c_str());
  exp_map_t::iterator iter;
  for (iter = _param_exps.begin(); iter != _param_exps.end(); ++iter) {
    tty->print("%d: ", iter->first);
    Expression *exp = iter->second;
    if (exp) {
      exp->print_cr();
    }
  }
}

bool SymStmt::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;

  if (callee_name == "execute") {
    int param_size = handle.get_callee_method()->size_of_parameters();
    assert(param_size == 2, "currently, we only support stmt.execute(String)");

    oop stmt_obj = handle.get_param<oop>(0);
    oop str_obj = handle.get_param<oop>(1);

    ResourceMark rm;
    const char *c_sql_template = OopUtils::java_string_to_c(str_obj);

    SymStmt *sym_stmt = (SymStmt *)ConcolicMngr::ctx->alloc_sym_inst(stmt_obj);
    sym_stmt->set_sql_template(c_sql_template);

    need_symbolize = true;
  } else if (callee_name == "executeQuery") {
    int param_size = handle.get_callee_method()->size_of_parameters();
    assert(param_size == 1, "currently, we only support stmt.executeQuery()");

    need_symbolize = true;
  } else if (skip_method_names.find(callee_name) != skip_method_names.end()) {
    need_symbolize = true;
  } else {
    tty->print_cr("SymStmt unhandle (%s): %s", handle.get_callee_holder_name().c_str(),
                  handle.get_callee_name().c_str());
    need_symbolize = true;
    // ShouldNotCallThis();
  }

  return need_symbolize;
}

Expression *SymStmt::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  if (callee_name == "executeQuery") {
    oop res_obj = handle.get_result<oop>(T_OBJECT);
    assert(handle.get_result_type() == T_OBJECT, "sanity check");
    assert(!res_obj->is_symbolic(), "please return a new resultset, JDBC!");

    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->alloc_sym_inst(res_obj);

    oop this_obj = handle.get_param<oop>(0);
    assert(this_obj->is_symbolic(), "should be");
    sym_res_set->set_stmt_rid(this_obj->get_sym_rid());
  } else if (callee_name == "execute") {
    tty->print_cr("%s: %s", handle.get_callee_holder_name().c_str(),
                  handle.get_callee_name().c_str());
  } else if (callee_name == "setInt") {
    oop stmt_obj = handle.get_param<oop>(0);
    jint index = handle.get_param<jint>(1);

    Expression *value_exp = ConcolicMngr::ctx->get_local_slot(
        handle.get_callee_local_begin_offset() +
        2); // this 2's unit is stack words!

    if (!value_exp) {
      value_exp = new ConExpression(handle.get_param<jint>(2));
    }

    SymStmt *sym_stmt = (SymStmt *)ConcolicMngr::ctx->get_sym_inst(stmt_obj);
    sym_stmt->set_param(index, value_exp);
  }
  return NULL;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC