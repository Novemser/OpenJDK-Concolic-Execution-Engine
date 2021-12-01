
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/utils.hpp"
#include "concolic/reference/symbolicString.hpp"

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
  set.insert("setBoolean");
  set.insert("setByte");
  set.insert("setInt");
  set.insert("setNull");
  set.insert("setString");
  set.insert("setShort");
  set.insert("setLong");
  set.insert("setFloat");
  set.insert("setDouble");
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

std::map<std::string, BasicType> SymStmt::support_set_methods = init_support_set_methods();

std::map<std::string, BasicType> SymStmt::init_support_set_methods() {
  std::map<std::string, BasicType> map;
  map["setBoolean"] = T_BOOLEAN;
  map["setByte"] = T_BYTE;
  map["setInt"] = T_INT;
  map["setShort"] = T_SHORT;
  map["setLong"] = T_LONG;
  map["setFloat"] = T_FLOAT;
  map["setDouble"] = T_DOUBLE;
  map["setString"] = T_OBJECT;
  map["setNull"] = T_OBJECT;
  return map;
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

    SymStmt *sym_stmt = (SymStmt *) ConcolicMngr::ctx->alloc_sym_inst(stmt_obj);
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
        (SymResSet *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);

    oop this_obj = handle.get_param<oop>(0);
    assert(this_obj->is_symbolic(), "should be");
    sym_res_set->set_stmt_rid(this_obj->get_sym_rid());
  } else if (callee_name == "execute") {
    tty->print_cr("%s: %s", handle.get_callee_holder_name().c_str(),
                  handle.get_callee_name().c_str());
  } else if (strncmp("set", callee_name.c_str(), 3) == 0) {
    ArgumentCount arg_cnt = ArgumentCount(handle.get_callee_method()->signature());
    if (arg_cnt.size() > 3) {
      tty->print_cr("Can't handle the set method having more than three params");
      ShouldNotReachHere();
    }
    if (SymStmt::support_set_methods.find(callee_name) != SymStmt::support_set_methods.end()) {
      oop stmt_obj = handle.get_param<oop>(0);
      jint index = handle.get_param<jint>(1);
      Expression *value_exp = SymStmt::get_param_exp(handle, support_set_methods[callee_name], callee_name);
      SymStmt *sym_stmt = (SymStmt *) ConcolicMngr::ctx->get_sym_inst(stmt_obj);
      sym_stmt->set_param(index, value_exp);
    } else {
      tty->print_cr("Unhandled set method:");
      handle.get_callee_method()->print_on(tty);
    }
  }
  return NULL;
}

// this type mean the param type of ? in sql statement
Expression *SymStmt::get_param_exp(MethodSymbolizerHandle &handle, BasicType type, const std::string &callee_name) {
  Expression *value_exp;
  int offset = 2;

  switch (type) {
    case T_BYTE:
      value_exp = new ConExpression(handle.get_param<jbyte>(offset));
      break;
    case T_BOOLEAN:
      value_exp = new ConExpression(handle.get_param<jboolean>(offset));
      break;
    case T_INT:
      value_exp = new ConExpression(handle.get_param<jint>(offset));
      break;
    case T_SHORT:
      value_exp = new ConExpression(handle.get_param<jshort>(offset));
      break;
    case T_LONG:
      value_exp = new ConExpression(handle.get_param<jlong>(offset + 1));
      break;
    case T_FLOAT:
      value_exp = new ConExpression(handle.get_param<jfloat>(offset));
      break;
    case T_DOUBLE:
      value_exp = new ConExpression(handle.get_param<jdouble>(offset + 1));
      break;
    case T_OBJECT:
      if (callee_name == "setNull") {
        value_exp = SymbolExpression::get(Sym_NULL);
      } else if (callee_name == "setString") {
        value_exp = SymString::get_exp_of(handle.get_param<oop>(offset));
      } else {
        ShouldNotReachHere();
      }
      break;
    default:
      ShouldNotReachHere();
  }
  return value_exp;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC