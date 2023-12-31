
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/reference/symbolicBigDecimal.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/utils.hpp"
#include "concolic/jdbc/jdbcUtils.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/reference/symbolicTimestamp.hpp"
#include "concolic/reference/symbolicDate.hpp"

long SymStmt::execute_counter = 0;
int SymStmt::_global_query_id = 0;
#ifdef ENABLE_WEBRIDGE
std::string SymStmt::_webridge_recorder_class = "edu/sjtu/ipads/wbridge/sql/ResultSetHolder";
#endif

std::set<std::string> SymStmt::target_class_names = init_target_class_names();

std::set<std::string> SymStmt::init_target_class_names() {
  std::set<std::string> set;
  set.insert("com/mysql/jdbc/JDBC42PreparedStatement");
  set.insert("com/mysql/jdbc/PreparedStatement");
  set.insert("com/mysql/jdbc/StatementImpl");
  set.insert("TestBigDecimal$StubPreparedStmt");// for test purpose only
  set.insert("TestBigDecimal$StubStmt");// for test purpose only
#ifdef ENABLE_WEBRIDGE
  set.insert(_webridge_recorder_class);// for recorded trace execution
#endif
  return set;
}

std::set<std::string> SymStmt::skip_method_names = init_skip_method_names();

std::set<std::string> SymStmt::init_skip_method_names() {
  std::set<std::string> set;
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

void SymStmt::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}

std::map<std::string, BasicType> SymStmt::support_set_methods = init_support_set_methods();

std::map<std::string, BasicType> SymStmt::init_support_set_methods() {
  std::map<std::string, BasicType> map;
  map["setBoolean"] = T_BOOLEAN;
  map["setByte"] = T_BYTE;
  map["setBytes"] = T_ARRAY;
  map["setInt"] = T_INT;
  map["setShort"] = T_SHORT;
  map["setLong"] = T_LONG;
  map["setFloat"] = T_FLOAT;
  map["setDouble"] = T_DOUBLE;
  map["setString"] = T_OBJECT;
  map["setNull"] = T_OBJECT;
  map["setTimestamp"] = T_OBJECT;
  map["setDate"] = T_OBJECT;
  map["setBigDecimal"] = T_OBJECT;
  map["setCharacterStream"] = T_OBJECT;
  return map;
}

SymStmt::SymStmt(sym_rid_t sym_rid) : SymInstance(sym_rid), _sql_template(""), _row_count_exp(NULL),_row_count(0), obj_rid(NULL_SYM_RID), _result_set(NULL), _query_id(++_global_query_id) {}

SymStmt::~SymStmt() {
  exp_map_t::iterator iter;
  for (iter = _param_exps.begin(); iter != _param_exps.end(); ++iter) {
    Expression::gc(iter->second);
  }
  Expression::gc(_row_count_exp);
}

void SymStmt::set_param(int index, Expression *exp) {
  exp->inc_ref();
  _param_exps.insert(std::make_pair(index, exp));
}

void SymStmt::set_row_count_exp(Expression *row_count_exp, int row_count) {
  this->_row_count_exp = row_count_exp;
  this->_row_count_exp->inc_ref();
  this->_row_count = row_count;
}

void SymStmt::print() {
  tty->print_cr("SymStmt(%lu) of txn (%lu): %s", _sym_rid, _tx_id, _sql_template.c_str());
  exp_map_t::iterator iter;
  for (iter = _param_exps.begin(); iter != _param_exps.end(); ++iter) {
    tty->print("%d: ", iter->first);
    Expression *exp = iter->second;
    if (exp) {
      exp->print_cr();
    }
  }
  if (_row_count_exp) {
    tty->print("row_count: %d, ", _row_count);
    _row_count_exp->print();
    tty->cr();
  }
  _pc.print();
}

bool SymStmt::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  ResourceMark rm;
  tty->print_cr("SymStmt.invoke_method_helper: Invoking %s", callee_name.c_str());

  if (callee_name == "execute") {
    const int param_size = handle.get_callee_method()->size_of_parameters();
    const std::string holder_name = handle.get_callee_holder_name();
    if (holder_name.find("Prepared") != std::string::npos) {
        if (param_size != 1) {
            guarantee(false, "currently, we only support preparedStatement.execute() with 0 parameter");
        }
        execute_counter++;
    } else {
        if (param_size != 2) {
            guarantee(param_size == 2, "currently, we only support stmt.execute(String)");
        }

        oop stmt_obj = handle.get_param<oop>(0);
        oop str_obj = handle.get_param<oop>(1);

        ResourceMark rm;
        const char *c_sql_template = OopUtils::java_string_to_c(str_obj);

        SymStmt *sym_stmt;
        if (stmt_obj->is_symbolic()) {
            tty->print_cr("Statement(execute) with %lu", stmt_obj->get_sym_rid());
            sym_stmt = (SymStmt *) ConcolicMngr::ctx->get_sym_inst(stmt_obj);
        } else {
            tty->print_cr("Statement(execute) with new statement");
            sym_stmt = (SymStmt *) ConcolicMngr::ctx->alloc_sym_inst(stmt_obj);
        }
        sym_stmt->set_sql_template(c_sql_template);
    }
  } else if (callee_name == "executeQuery" || callee_name == "executeUpdate") {
    int param_size = handle.get_callee_method()->size_of_parameters();
    execute_counter++;
#ifdef ENABLE_WEBRIDGE
    if (handle.get_callee_holder_name() != _webridge_recorder_class) {
      if (handle.get_callee_holder_name() != "com/mysql/jdbc/StatementImpl") {
        guarantee(param_size == 1, "currently, we only support stmt.executeQuery()");
      } else {
        tty->print_cr("[unhandled query type] com.mysql.jdbc.StatementImpl.executeQuery, please use prepared statement instead for WeBridge to analysis");
      }
    } else {
      intptr_t * caller_locals =
          handle.get_caller_frame()->as_interpreter_frame()->interpreter_state()->locals();
      oop stmt_obj = *(oop *) (caller_locals - handle.get_caller_stack_begin_offset());
      ResourceMark rm;
      char* stmt_clz_name = stmt_obj->klass()->name()->as_C_string();
      guarantee(target_class_names.find(stmt_clz_name) != target_class_names.end(),
                (std::string("unexpected obj class type:") + stmt_clz_name).c_str());
      assert(param_size == 1, "should be 1");
      oop str_obj = handle.get_param<oop>(0);
      assert(str_obj->klass()->name() == vmSymbols::java_lang_String(), "must be string query string");
      const char *c_sql_template = OopUtils::java_string_to_c(str_obj);
      SymStmt *sym_stmt;
      if (stmt_obj->is_symbolic()) {
        sym_stmt = reinterpret_cast<SymStmt *>(ConcolicMngr::ctx->get_sym_inst(stmt_obj));
      } else {
        sym_stmt = reinterpret_cast<SymStmt *>(ConcolicMngr::ctx->alloc_sym_inst(stmt_obj));
      }
      sym_stmt->set_sql_template(c_sql_template);
    }
  } else if (callee_name == "doTxnRelatedSQL") {
    assert(handle.get_caller_frame()->is_interpreter_frame(), "must be");
    ResourceMark rm;
    intptr_t * caller_locals =
        handle.get_caller_frame()->as_interpreter_frame()->interpreter_state()->locals();
    oop conn_obj = *(oop *) (caller_locals - handle.get_caller_stack_begin_offset());
//    tty->print_cr("!!!ResultSetHolder conn_obj clz is %s", conn_obj->klass()->signature_name());
    long conn_id = JdbcUtils::get_conn_connection_id(conn_obj);
    oop query_str_oop = handle.get_param<oop>(0);
    std::string query_str = java_lang_String::as_utf8_string(query_str_oop);
//    tty->print_cr("received query str %s", query_str.c_str());
    if (query_str == "commit") {
      ConcolicMngr::ctx->get_jdbc_mngr().commit(conn_id);
    } else if (query_str == "set autocommit=1") {
      ConcolicMngr::ctx->get_jdbc_mngr().set_auto_commit(true, conn_id);
    } else if (query_str == "set autocommit=0") {
      ConcolicMngr::ctx->get_jdbc_mngr().set_auto_commit(false, conn_id);
    } else {
      guarantee(false, query_str.c_str());
    }
    need_symbolize = false;
#else
    guarantee(param_size == 1, "currently, we only support stmt.executeQuery()");
#endif
  } else if (callee_name == "setObject" ||
              callee_name == "setNumericObject" ||
              callee_name == "setNString") {
    // inside set object, jdbc invokes setXXX
    need_symbolize = false;
  } else if (SymStmt::support_set_methods.find(callee_name) != SymStmt::support_set_methods.end()) {
    ArgumentCount arg_cnt = ArgumentCount(handle.get_callee_method()->signature());
    if (arg_cnt.size() > 3) {
      tty->print_cr("Can't handle the set method having more than three params");
      ShouldNotReachHere();
    }
    oop stmt_obj = handle.get_param<oop>(0);
    jint index = handle.get_param<jint>(1);
    Expression *value_exp = SymStmt::get_param_exp(handle, support_set_methods[callee_name], callee_name);
    SymStmt *sym_stmt = reinterpret_cast<SymStmt *>(ConcolicMngr::ctx->get_or_alloc_sym_inst(stmt_obj));
    sym_stmt->set_param(index, value_exp);
  } else if (skip_method_names.find(callee_name) == skip_method_names.end()) {
//    handle.get_callee_method()->print_name(tty);
    // ShouldNotCallThis();
  } else {
    tty->print_cr("[SymbolicStatement] %s unhandled by SymStmt", callee_name.c_str());
  }

  return need_symbolize;
}

Expression *SymStmt::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;

  if (strncmp("execute", callee_name.c_str(), 7) == 0) {
#ifdef ENABLE_WEBRIDGE
    oop this_obj;
    if (handle.get_callee_holder_name() == _webridge_recorder_class) {
      assert(handle.get_caller_frame()->is_interpreter_frame(), "must be");
      ResourceMark rm;
      intptr_t *caller_locals =
          handle.get_caller_frame()->as_interpreter_frame()->interpreter_state()->locals();
      this_obj = *(oop *) (caller_locals - handle.get_caller_stack_begin_offset());
      guarantee(!strcmp(this_obj->klass()->signature_name(), "Lcom/mysql/jdbc/JDBC42PreparedStatement;"), "should be");
    } else {
      this_obj = handle.get_param<oop>(0);
    }
#else
    oop this_obj = handle.get_param<oop>(0);
#endif
    SymStmt *sym_stmt = reinterpret_cast<SymStmt *>(ConcolicMngr::ctx->get_or_alloc_sym_inst(this_obj));
    // set current path conditon for statement
    sym_stmt->set_pc(ConcolicMngr::ctx->get_path_condition());
//    ConcolicMngr::ctx->record_path_condition(new StatementSymbolExp(sym_stmt));
    jlong conn_id = JdbcUtils::get_stmt_connection_id(this_obj);
    ConcolicMngr::ctx->get_jdbc_mngr().record_stmt(sym_stmt, conn_id);

    if (callee_name == "executeQuery") {
      oop res_obj = handle.get_result<oop>(T_OBJECT);
      SymResSet *sym_res_set =
          (SymResSet *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);
      sym_res_set->set_sym_stmt(sym_stmt);
      sym_stmt->set_result_set(sym_res_set);
    } else if (callee_name == "executeUpdate") {
      jint row_count = handle.get_result<jint>(T_INT);
      exp = new ResultSetSymbolExp(sym_stmt);
      sym_stmt->set_row_count_exp(exp, row_count);
    } else if (callee_name == "execute") {
      jboolean exe_res = handle.get_result<jboolean>(T_BOOLEAN);
      tty->print_cr("execute()_res:%d", exe_res);
    }
  } else if (callee_name == "getResultSet") {
    oop this_obj = handle.get_param<oop>(0);
    SymStmt *sym_stmt = reinterpret_cast<SymStmt *>(ConcolicMngr::ctx->get_or_alloc_sym_inst(this_obj));
    oop res_obj = handle.get_result<oop>(T_OBJECT);
    SymResSet *sym_res_set =
          (SymResSet *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);
    sym_res_set->set_sym_stmt(sym_stmt);
    if (sym_stmt->get_result_set() != NULL) {
        guarantee(false, "currently, we did not handle getResultSet() returning multiple result sets");
    }
    sym_stmt->set_result_set(sym_res_set);
  } else if (callee_name == "getUpdateCount") {
    oop this_obj = handle.get_param<oop>(0);
    SymStmt *sym_stmt = reinterpret_cast<SymStmt *>(ConcolicMngr::ctx->get_or_alloc_sym_inst(this_obj));
    if (sym_stmt->is_write_sql()) {
      jint row_count = handle.get_result<jint>(T_INT);
      exp = new ResultSetSymbolExp(sym_stmt);
      sym_stmt->set_row_count_exp(exp, row_count);
    }
  }
  return exp;
}

// this type mean the param type of ? in sql statement
Expression *SymStmt::get_param_exp(MethodSymbolizerHandle &handle, BasicType type, const std::string &callee_name) {
  int offset = 2;
  Expression *value_exp;
  if (is_java_primitive(type)) {
    value_exp = handle.get_primitive_exp(offset, type);
  } else if (callee_name == "setString") {
    value_exp = SymString::get_exp_of(handle.get_param<oop>(offset));
  } else if (callee_name == "setNull") {
    value_exp = SymbolExpression::get(Sym_NULL);
  } else if (callee_name == "setTimestamp") {
    value_exp = SymTimestamp::get_exp_of(handle.get_param<oop>(offset));
  } else if (callee_name == "setDate") {
    value_exp = SymbolicDate::get_exp_of(handle.get_param<oop>(offset));
  } else if (callee_name == "setBigDecimal") {
    value_exp = SymBigDecimal::get_exp_of(handle.get_param<oop>(offset));
  } else if (callee_name == "setCharacterStream") {
    ResourceMark rm;
    oop reader_obj = handle.get_param<oop>(offset);
    const std::string &cname = std::string(reader_obj->klass()->name()->as_C_string());
    if (cname == "java/io/StringReader") {
      oop str_obj = reader_obj->obj_field(56);
      value_exp = SymString::get_exp_of(str_obj);
    } else {
      guarantee(false, (std::string("Not implemented setCharacterStream.cname:") + cname).c_str());
    }
  } else if (callee_name == "setBytes") {
    value_exp = ArrayExpression::get_exp_of(handle.get_param<oop>(offset));
  } else {
    guarantee(false, (std::string("Not implemented callee:") + callee_name).c_str());
  }
  return value_exp;
}

const exp_map_t &SymStmt::get_param_exps() const {
  return _param_exps;
}

Expression *SymStmt::get_ref_exp() {
  return NULL;
}

SymResSet *SymStmt::get_result_set() const {
  return _result_set;
}

void SymStmt::set_result_set(SymResSet *resultSet) {
  _result_set = resultSet;
}

void SymStmt::set_sym_exp(int field_offset, Expression *exp) {
  ShouldNotCallThis();
}

StatementSymbolExp::StatementSymbolExp(SymStmt *sym_stmt) : _sym_stmt(sym_stmt) {};

void StatementSymbolExp::print() {
  tty->print("#SymStmt(%lu): %s", _sym_stmt->get_sym_rid(), _sym_stmt->_sql_template.c_str());
}

void StatementSymbolExp::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  ShouldNotCallThis();
}

void SymSetAutoCommit::print() {
  tty->print_cr("#setAutocommit=%d", getAutoCommit());
}

bool SymSetAutoCommit::is_txn_control() {
  return true;
}

void SymSetAutoCommit::set_sym_exp(int field_offset, Expression *exp) {

}

void SymCommit::print() {
  tty->print_cr("#commit");
}

bool SymCommit::is_txn_control() {
  return true;
}

void SymCommit::set_sym_exp(int field_offset, Expression *exp) {

}

bool SymStmt::is_write_sql() {
  if (_sql_template.length() == 0) return false;
  // TODO refactor
  if (!_sql_template.rfind("UPDATE", 0) || !_sql_template.rfind("update", 0)) return true;
  if (!_sql_template.rfind("INSERT", 0) || !_sql_template.rfind("insert", 0)) return true;
  if (!_sql_template.rfind("DELETE", 0) || !_sql_template.rfind("delete", 0)) return true;
  return false;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC