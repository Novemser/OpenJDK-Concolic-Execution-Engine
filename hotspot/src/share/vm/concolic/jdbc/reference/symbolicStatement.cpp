
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/jdbc/reference/symbolicResultSet.hpp"

// const char *SymStmt::TYPE_NAME = "com/mysql/jdbc/StatementImpl";
const char *SymStmt::TYPE_NAME = "com/mysql/jdbc/JDBC42PreparedStatement";
const char *SymStmt::BASE_TYPE_NAME = "com/mysql/jdbc/PreparedStatement";

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

bool SymStmt::invoke_method(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;

  if (callee_name == "setInt") {
    oop stmt_obj = handle.get_param<oop>(0);
    jint index = handle.get_param<jint>(1);

    Expression *value_exp = ConcolicMngr::ctx->get_stack_slot(
        handle.get_caller_stack_begin_offset() + 2);
    if (!value_exp) {
      value_exp = new ConExpression(handle.get_param<jint>(2));
    }

    SymStmt *sym_stmt = (SymStmt *)ConcolicMngr::ctx->get_sym_inst(stmt_obj);
    sym_stmt->set_param(index, value_exp);

    need_symbolize = true;
  } else if (callee_name == "executeQuery") {
    int param_size = handle.get_callee_method()->size_of_parameters();
    assert(param_size == 1, "currently, we only support stmt.executeQuery()");

    need_symbolize = true;
  } else {
    ShouldNotCallThis();
  }

  return need_symbolize;
}

void SymStmt::finish_method(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  if (callee_name == "executeQuery") {
    oop res_obj = handle.get_result<oop>();
    assert(handle.get_result_type() == T_OBJECT, "sanity check");
    assert(!res_obj->is_symbolic(), "please return a new resultset, JDBC!");
    assert(res_obj->klass()->name()->equals(SymResSet::TYPE_NAME), "should be");

    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->alloc_sym_inst(res_obj);

    oop this_obj = handle.get_param<oop>(0);
    assert(this_obj->is_symbolic(), "should be");
    sym_res_set->set_stmt_rid(this_obj->get_sym_rid());
  }
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC