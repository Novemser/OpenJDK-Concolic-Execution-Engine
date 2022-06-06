
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/SymbolicOp.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/utils.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "concolic/reference/symbolicBigDecimal.hpp"

std::set<std::string> SymResSet::target_class_names = init_target_class_names();

std::set<std::string> SymResSet::init_target_class_names() {
  std::set<std::string> set;
  set.insert("com/mysql/jdbc/JDBC42ResultSet");
  set.insert("com/mysql/jdbc/ResultSetImpl");
//  set.insert("TestBigDecimal$StubResultSet");
  return set;
}

std::set<std::string> SymResSet::handle_method_names =
    init_handle_method_names();

std::set<std::string> SymResSet::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("getBoolean");
  set.insert("getByte");
  set.insert("getShort");
  set.insert("getInt");
  set.insert("getLong");
  set.insert("getFloat");
  set.insert("getDouble");
  set.insert("getDate");
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
  set.insert("findColumn");
  set.insert("checkRowPos");
  set.insert("checkClosed");
  set.insert("getInstance");
  set.insert("getMetaData");
  set.insert("$jacocoInit");
  set.insert("checkColumnBounds");
  // do not handle clob~
  set.insert("getClob");
  set.insert("wasNull");  // TODO: check this
  return set;
}

void SymResSet::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}

SymResSet::SymResSet(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _sym_stmt(NULL), _ref_exp(NULL), _sql_id(0),
      _row_id(0), _last_got_index(-1) {}

SymResSet::~SymResSet() {
  Expression::gc(_ref_exp);
}

void SymResSet::print() {
  tty->print_cr("SymResSet of SymStmt %lu", _sym_stmt->get_sym_rid());
}

void SymResSet::set_sym_stmt(SymStmt *sym_stmt) {
  _sym_stmt = sym_stmt;
  _sql_id = sym_stmt->get_query_id();
  _ref_exp = new ResultSetSymbolExp(this);
  _ref_exp->inc_ref();
  sym_stmt->set_row_count_exp(new ResultSetSymbolExp(this, true), 0);
}

bool SymResSet::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;

  // WANING: Do not invoke ConcolicMngr::ctx->get_sym_inst(this) on static methods
//  tty->print_cr("invoke_method_helper:%s", callee_name.c_str());
  if (callee_name == "next") {
    oop res_set_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(res_set_obj);
    sym_res_set->next();
    need_symbolize = true;
  } else if (callee_name == "getObject") {
    // inside getObject, getInt/Long/String etc. will be invoked
    need_symbolize = false;
  } else if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    oop res_set_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(res_set_obj);

    oop this_obj = res_set_obj;
    BasicType col_type;
    {
      ResourceMark rm;
      SignatureStream ss(handle.get_callee_method()->signature());
      col_type = ss.type();
    }

    if (col_type != T_OBJECT) {
      return need_symbolize = true;
    }
    oop col_str_obj = handle.get_param<oop>(1);
    JavaThread* thread = JavaThread::current();
    guarantee(col_str_obj->klass()->name()->equals(SymString::TYPE_NAME),
              "should be string");

    // use index to name the symbolic result set result
    // prepare calling parameters
    //  - calling param: 1. resultSet obj 2. the same string as calling the getXXX. They are already ready in stack.
    JavaValue res(T_INT);
    KlassHandle klass(thread, this_obj->klass());
    const char* signature = "(Ljava/lang/String;)I";
    const char* func_name = "findColumn";
    JavaThreadState lastState = thread->thread_state();
    // must transfer thread state to state_VM
    thread->set_thread_state(_thread_in_vm);
    tty->print_cr("Calling virtual %s.findColumn,result set class:%s", handle.get_callee_holder_name().c_str(),
                  this_obj->klass()->name()->as_C_string());
    // temporally disable concolic execution flag to avoid corruption of MethodSymbolizerHandle
    ThreadContext* tc = ConcolicMngr::ctx;
    ConcolicMngr::ctx = NULL;
    JavaCalls::call_virtual(
        &res,
        Handle(this_obj),
        klass,
        SymbolTable::lookup(func_name, (int) strlen(func_name), thread),
        SymbolTable::lookup(signature, (int) strlen(signature), thread),
        Handle(col_str_obj),
        thread
    );
    ConcolicMngr::ctx = tc;
    // don't forget to restore the thread state
    thread->set_thread_state(lastState);
    assert(sym_res_set->_last_got_index == -1, "Should not be any pending index recording");
    sym_res_set->_last_got_index = res.get_jint();
    need_symbolize = true;
  } else if (skip_method_names.find(callee_name) != skip_method_names.end()) {
    need_symbolize = true;
  } else {
    handle.get_callee_method()->print_name(tty);
    tty->print_cr("%s should handled by SymResSet", handle.get_callee_name().c_str());
//    ShouldNotCallThis();
  }

  return need_symbolize;
}

Expression *SymResSet::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    oop this_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(this_obj);

    BasicType col_type;
    {
      ResourceMark rm;
      SignatureStream ss(handle.get_callee_method()->signature());
      col_type = ss.type();
    }

    BasicType res_type = handle.get_result_type();
    oop res_obj = NULL;
    const char* res_obj_signature_name = NULL;
    if (res_type == T_OBJECT) {
      res_obj = handle.get_result<oop>(T_OBJECT);
      if (res_obj != NULL) {
        res_obj_signature_name = res_obj->klass()->signature_name();
      }
    }

    jint col_i;
    if (col_type == T_OBJECT) {
      oop col_str_obj = handle.get_param<oop>(1);
      guarantee(col_str_obj->klass()->name()->equals(SymString::TYPE_NAME),
                "should be");

//      const char *col_name = OopUtils::java_string_to_c(col_str_obj);
      assert(sym_res_set->_last_got_index != -1, "Should set correctly");
//      exp = new ResultSetSymbolExp(sym_res_set, res.get_jint(), res_type, res_obj);
      // multiple String name could corresponds to one column in the result, this can cause ambiguous symbolic names.
      // use index as unique identifier!
      col_i = sym_res_set->_last_got_index;
      sym_res_set->_last_got_index = -1;
    } else if (col_type == T_INT) {
      col_i = handle.get_param<int>(1);
    } else {
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" should handled by SymResSet");
      ShouldNotCallThis();
    }

    if (res_obj_signature_name == NULL) {
      exp = new ResultSetSymbolExp(sym_res_set, col_i, res_type, res_obj);
    } else {
      // TODO: result variable different object type handling
      // 1. String 2. bigdecimal 3. date/time
      std::string res_tp = res_obj_signature_name;
      std::string exp_name;
      sym_res_set->get_ret_val_name(col_i, exp_name);
      guarantee(!exp_name.empty(), "should not be empty exp_name");

      if (res_tp == "Ljava/lang/String;") {
        SymString *sym_str = reinterpret_cast<SymString *>(
            ConcolicMngr::ctx->get_or_alloc_sym_inst(res_obj)
        );
        StringSymbolExp *sym_str_exp = reinterpret_cast<StringSymbolExp *>(
            sym_str->get_ref_exp()
        );
        sym_str_exp->set(exp_name.c_str(), (int) exp_name.length());
        exp = sym_str_exp;
      } else if (res_tp == "Ljava/math/BigDecimal;") {
        SymBigDecimal *sym_bd = reinterpret_cast<SymBigDecimal *>(
            ConcolicMngr::ctx->get_or_alloc_sym_inst(res_obj)
        );
        sym_bd->set_bigDecimal_symbolic(res_obj, exp_name);
      } else if (res_tp == "Ljava/sql/Timestamp;" || res_tp == "Ljava/sql/Date;") {
        ConcolicMngr::ctx->symbolize_recursive(res_obj);
      } else {
        tty->print_cr("Not implement %s", res_obj_signature_name);
        ShouldNotCallThis();
      }
    }
  } else if (skip_method_names.find(callee_name) != skip_method_names.end()) {
    // DO NOTHING ABOUT skipped method
    return NULL;
  } else if (callee_name == "next") {
    oop this_obj = handle.get_param<oop>(0);
    SymResSet *sym_res_set =
        (SymResSet *)ConcolicMngr::ctx->get_sym_inst(this_obj);
    jboolean ret = handle.get_result<jboolean>(T_BOOLEAN);

    if (ret) {
      sym_res_set->get_sym_stmt()->inc_row_count();
    }

    ConcolicMngr::record_path_condition(new OpSymExpression(
        sym_res_set->get_sym_stmt()->get_row_count_exp(),
        new ConExpression(sym_res_set->get_row_id()), op_ge, ret));
  }

  return exp;
}

SymStmt *SymResSet::get_sym_stmt() { return _sym_stmt; }

void SymResSet::set_sym_exp(int field_offset, Expression *exp) {
  if (exp != NULL) {
    tty->print_cr("Setting symRes field offset:%d", field_offset);
    exp->print_cr();
    ShouldNotCallThis();
  }
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set, bool is_size) {
  stringStream ss(str_buf, BUF_SIZE);
  if (is_size) {
    _type = type2char(T_INT);
    set_head(ss, 'M', T_INT);
    ss.print("RS_%lu_size", sym_res_set->_sql_id);
  } else {
    _type = type2char(T_OBJECT);
    set_head(ss, 'M', T_OBJECT, "'ResultSet'");
    ss.print("RS_%lu", sym_res_set->_sql_id);
  }
  this->finalize(ss.size());
}

ResultSetSymbolExp::ResultSetSymbolExp(SymStmt *sym_stmt) {
  stringStream ss(str_buf, BUF_SIZE);
  _type = type2char(T_INT);
  set_head(ss, 'M', T_INT);
  ss.print("RS_%lu_rowCount", sym_stmt->get_sym_rid());
  this->finalize(ss.size());
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set,
                                       const char *col_name, BasicType type,
                                       oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type, obj);
  ss.print("RS_q%lu_r%d_%s", sym_res_set->_sql_id, sym_res_set->_row_id,
           col_name);
  this->finalize(ss.size());
}

ResultSetSymbolExp::ResultSetSymbolExp(SymResSet *sym_res_set, int col_i,
                                       BasicType type, oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  if (obj != NULL) {
    _type = obj->klass()->name()->as_C_string();
  } else {
    _type = type2char(type);
  }
  set_head(ss, 'M', type, obj);
  ss.print("RS_q%lu_r%d_col%d", sym_res_set->_sql_id, sym_res_set->_row_id,
           col_i);
  this->finalize(ss.size());
}

// TODO: refactor ResultSetSymbolExp ctor to use the same name generation logic
void SymResSet::get_ret_val_name(int col_i, std::string &input) {
  const int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE];
  stringStream ss(buffer, BUFFER_SIZE);
  ss.print("RS_q%lu_r%d_col%d", this->_sql_id, this->_row_id, col_i);
  std::string val = std::string(buffer, ss.size());
  input.swap(val);
}

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC