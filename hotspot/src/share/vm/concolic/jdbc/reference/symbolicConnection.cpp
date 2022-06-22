#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicConnection.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/utils.hpp"
#include "concolic/jdbc/jdbcUtils.hpp"

const char *SymConn::TYPE_NAME = "com/mysql/jdbc/ConnectionImpl";
std::string SymConn::sql_template = "";

method_set_t SymConn::handle_method_names = init_handle_method_names();

method_set_t SymConn::init_handle_method_names() {
  method_set_t m_set;
  m_set.insert("prepareStatement");
  m_set.insert("createStatement");
  m_set.insert("setAutoCommit");
  m_set.insert("commit");
  return m_set;
}


method_set_t SymConn::skip_method_names = init_skip_method_names();

method_set_t SymConn::init_skip_method_names() {
  method_set_t m_set;
  m_set.insert("getAutoCommit");
  m_set.insert("getProxy");
  m_set.insert("getServerCharset");
  m_set.insert("getCharsetConverter");
  return m_set;
}

void SymConn::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymConn::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymConn::TYPE_NAME, finish_method_helper);
}

bool SymConn::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
//  tty->print_cr("invoke_method_helper callee_name in sym Connection:%s", callee_name.c_str());
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    if (callee_name == "prepareStatement") {
      oop obj = handle.get_param<oop>(1);
      assert(obj->klass()->name()->equals(SymString::TYPE_NAME), "should be");
      assert(sql_template.empty(), "empty");

      ResourceMark rm;
      sql_template = std::string(OopUtils::java_string_to_c(obj));
    } else if (callee_name == "setAutoCommit") {
      long conn_id = JdbcUtils::get_conn_connection_id(handle.get_param<oop>(0));
      jboolean auto_commit = handle.get_param<jboolean>(1);
      ConcolicMngr::ctx->get_jdbc_mngr().set_auto_commit(auto_commit, conn_id);
    } else if (callee_name == "createStatement") {
    } else if (callee_name == "commit") {
      long conn_id = JdbcUtils::get_conn_connection_id(handle.get_param<oop>(0));
      tty->print_cr("Connection[%lu] calling commit", conn_id);
      ConcolicMngr::ctx->get_jdbc_mngr().commit(conn_id);
    } else {
      ShouldNotCallThis();
    }
    need_symbolize = true;
  } else if (skip_method_names.find(callee_name) != skip_method_names.end()) {
    need_symbolize = true;
  }
//  else {
////    handle.get_callee_method()->print_name(tty);
////    tty->print_cr(" handled by SymConn");
//  }

  return need_symbolize;
}

Expression *SymConn::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
//  tty->print_cr("finish_method_helper callee_name in sym Connection:%s", callee_name.c_str());
  if (callee_name == "prepareStatement") {
    oop res_obj = handle.get_result<oop>(T_OBJECT);

    SymStmt *sym_stmt = (SymStmt *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);
    sym_stmt->swap_sql_template(sql_template);
//    tty->print_cr("Connection prepare statement: %lu", sym_stmt->get_sym_rid());
  } else if (callee_name == "createStatement") {
    oop res_obj = handle.get_result<oop>(T_OBJECT);
    SymStmt *sym_stmt = (SymStmt *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);
//    tty->print_cr("Connection create statement: %lu", sym_stmt->get_sym_rid());
  }

  return NULL;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
