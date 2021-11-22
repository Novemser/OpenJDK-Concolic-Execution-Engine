#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicConnection.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/utils.hpp"

const char *SymConn::TYPE_NAME = "com/mysql/jdbc/ConnectionImpl";
std::string SymConn::sql_template = "";
method_set_t SymConn::symbolized_methods = init_symbolized_methods();

method_set_t SymConn::init_symbolized_methods() {
  method_set_t m_set;
  m_set.insert("prepareStatement");
  return m_set;
}

bool SymConn::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
    /**
     * Currently, this is only for prepareStatement
     */
    int offset = handle.get_callee_local_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();
    Method *callee_method = handle.get_callee_method();

    assert(!callee_method->is_static(), "should be");
    // handle this
    ++offset;

    oop obj = *(oop *)(locals - offset);
    assert(obj->klass()->name()->equals(SymString::TYPE_NAME), "should be");
    assert(sql_template.empty(), "empty");

    ResourceMark rm;
    sql_template = std::string(OopUtils::java_string_to_c(obj));

    need_symbolize = true;
  }

  return need_symbolize;
}

Expression *SymConn::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  assert(symbolized_methods.find(callee_name) != symbolized_methods.end(),
         "sanity check");

  /**
   * Currently, this is only for prepareStatement
   */
  oop res_obj = handle.get_result<oop>();
  assert(handle.get_result_type() == T_OBJECT, "sanity check");
  assert(!res_obj->is_symbolic(), "please return a new statment, JDBC!");
  assert(res_obj->klass()->name()->equals(SymStmt::TYPE_NAME), "should be");
  assert(!sql_template.empty(), "empty");

  SymStmt *sym_stmt = (SymStmt *)ConcolicMngr::ctx->alloc_sym_inst(res_obj);
  sym_stmt->swap_sql_template(sql_template);

  return NULL;
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
