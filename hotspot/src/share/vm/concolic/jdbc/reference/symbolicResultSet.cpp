
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
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

std::set<std::string> SymResSet::handle_method_names = init_handle_method_names();

std::set<std::string> SymResSet::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("getBoolean");
  set.insert("getByte");
  set.insert("getShort");
  set.insert("getInt");
  set.insert("getLong");
  set.insert("getFloat");
  set.insert("getDouble");
  set.insert("getString");
  set.insert("getTimestamp");
  set.insert("getBigDecimal");
  return set;
}

std::set<std::string> SymResSet::skip_method_names = init_skip_method_names();

std::set<std::string> SymResSet::init_skip_method_names() {
  std::set<std::string> set;
  set.insert("close");
  set.insert("realClose");
  set.insert("checkRowPos");
  set.insert("checkClosed");
  set.insert("checkColumnBounds");
  // do not handle clob~
  set.insert("getClob");
  return set;
}

SymResSet::SymResSet(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _ref_exp(NULL), _sym_stmt_rid(0), _sql_id(0), _row_id(0) {}

SymResSet::~SymResSet() {
  Expression::gc(_size_exp);
  Expression::gc(_ref_exp);
}

void SymResSet::print() {
  tty->print_cr("SymResSet of SymStmt %lu", _sym_stmt_rid);
}

bool SymResSet::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;

  if (callee_name == "next") {
    oop res_set_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *) ConcolicMngr::ctx->get_sym_inst(res_set_obj);
    sym_res_set->next();

    need_symbolize = true;
  } else if (handle_method_names.find(callee_name) != handle_method_names.end() ||
             skip_method_names.find(callee_name) != skip_method_names.end()) {
    need_symbolize = true;
  } else {
//    oop str_obj = handle.get_param<oop>(1);
//    ResourceMark rm;
//    tty->print_cr("%s", OopUtils::java_string_to_c(str_obj));

    handle.get_callee_method()->print_name(tty);
    tty->print_cr(" handled by SymResSet");
     ShouldNotCallThis();
  }

  return need_symbolize;
}

Expression *SymResSet::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;
  if (strncmp("get", callee_name.c_str(), 3) == 0) {
    oop this_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *) ConcolicMngr::ctx->get_sym_inst(this_obj);

    BasicType col_type;
    {
      ResourceMark rm;
      SignatureStream ss(handle.get_callee_method()->signature());
      col_type = ss.type();
    }

    BasicType res_type = handle.get_result_type();
    oop res_obj = NULL;
    {
      if (res_type == T_OBJECT) {
        res_obj = handle.get_result<oop>(T_OBJECT);
      }
    }
    if (col_type == T_OBJECT) {
      oop col_str_obj = handle.get_param<oop>(1);
      guarantee(col_str_obj->klass()->name()->equals(SymString::TYPE_NAME),
                "should be");

      const char *col_name = OopUtils::java_string_to_c(col_str_obj);
      exp = new ResultSetSymbolExp(sym_res_set, col_name, res_type, res_obj);
    } else if (col_type == T_INT) {
      jint col_i = handle.get_param<int>(1);
      exp = new ResultSetSymbolExp(sym_res_set, col_i, res_type, res_obj);
    } else {
      ShouldNotCallThis();
    }

//    if (is_java_primitive(type)) {
//    } else if (type == T_OBJECT) {
//    }
  } else if (callee_name == "next") {
    oop this_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *) ConcolicMngr::ctx->get_sym_inst(this_obj);
    jboolean ret = handle.get_result<jboolean>(T_BOOLEAN);

    ConcolicMngr::record_path_condition(new OpSymExpression(
        sym_res_set->get_size_exp(),
        new ConExpression(sym_res_set->get_row_id()), op_ge, ret));
  }

  return exp;
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set, bool is_size) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', T_INT);
  ss.print("RS_%lu", sym_res_set->_sql_id);
  if (is_size) {
    ss.print("_size");
  }
  this->finalize(ss.size());
}

ResultSetSymbolExp::ResultSetSymbolExp(SymStmt *sym_stmt) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', T_INT);
  ss.print("RS_%lu_rowCount", sym_stmt->get_sym_rid());
  this->finalize(ss.size());
}


ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set,
                                       const char *col_name, BasicType type, oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type, obj);
  ss.print("RS_%lu_%d_%s", sym_res_set->_sql_id,
           sym_res_set->_row_id, col_name);
  this->finalize(ss.size());
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set,
                                       int col_i, BasicType type, oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type, obj);
  ss.print("RS_%lu_%d_col%d", sym_res_set->_sql_id,
           sym_res_set->_row_id, col_i);
  this->finalize(ss.size());
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC