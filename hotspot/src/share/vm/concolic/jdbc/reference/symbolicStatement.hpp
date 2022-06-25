#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/pathCondition.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "symbolicResultSet.hpp"

class SymStmt;

class StatementSymbolExp : public Expression {
private:
  SymStmt *_sym_stmt;
public:
  StatementSymbolExp(SymStmt *sym_stmt);

protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

public:
  void print();
};

class SymStmt : public SymInstance {
  friend class StatementSymbolExp;

public:
  inline static bool target(const std::string &class_name) {
    return target_class_names.find(class_name) != target_class_names.end();
  }

private:
  static int _global_query_id;
#ifdef ENABLE_WEBRIDGE
  static std::string _webridge_recorder_class;
#endif
  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> skip_method_names;
  static std::set<std::string> init_skip_method_names();
  static std::map<std::string, BasicType> support_set_methods;
  static std::map<std::string, BasicType> init_support_set_methods();
  static Expression *get_param_exp(MethodSymbolizerHandle &handle, BasicType method, const std::string &callee_name);

private:
  tx_id_t _tx_id;
  std::string _sql_template;
  exp_map_t _param_exps;
  Expression *_row_count_exp;
  int _row_count;
  sym_rid_t obj_rid;
  PathCondition _pc;
  SymResSet* _result_set;
  int _query_id;

public:
  SymStmt(sym_rid_t sym_rid);
  ~SymStmt();

public:
  const exp_map_t &get_param_exps() const;

  inline std::string &get_sql_template() {
    return _sql_template;
  }

  inline void swap_sql_template(std::string &sql_template) {
    _sql_template.swap(sql_template);
  }

  inline void set_sql_template(const char *sql_template) {
    _sql_template = std::string(sql_template);
  }

  inline void set_tx_id(tx_id_t tx_id) { _tx_id = tx_id; }

  void set_param(int index, Expression *exp);

  inline Expression *get_row_count_exp() { return _row_count_exp; }

  void set_row_count_exp(Expression *row_count_exp, int row_count);

  inline void inc_row_count() { ++ _row_count;}

  sym_rid_t get_obj_rid() { return obj_rid; }
  sym_rid_t set_obj_rid(sym_rid_t id) {
    guarantee(obj_rid == NULL_SYM_RID, "should be");
    obj_rid = id;
  }

  PathCondition getPc() const { return _pc; }

  void set_pc(PathCondition pc) { _pc = pc; }

  virtual bool is_txn_control() { return false; }

  virtual Expression *get_ref_exp();

  SymResSet *get_result_set() const;

  void set_result_set(SymResSet *resultSet);

  inline int get_query_id() { return _query_id; }

  inline int get_concrete_row_count() { return _row_count; }
public:
  bool need_recursive() { return false; }
  void print();

  virtual void set_sym_exp(int field_offset, Expression *exp);
  bool is_write_sql();
public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void init_register_class(MethodSymbolizer* m_symbolizer);

// counter: for symbolicPersister
private:
  static long execute_counter;
public:
  static long getExecuteCounter() { return execute_counter; }
  static long resetExecuteCounter() { execute_counter = 0; }
  static long resetGlobalCounter() { _global_query_id = 0; }
};

class SymSetAutoCommit : public SymStmt {
private:
  bool autoCommit;
public:
  SymSetAutoCommit(bool autoCommit) : SymStmt(Sym_NULL) {
    this->autoCommit = autoCommit;
    if (autoCommit) {
      set_sql_template("set autocommit=true");
    } else {
      set_sql_template("set autocommit=false");
    }
  }

  bool getAutoCommit() {
    return autoCommit;
  }

  void print();

  virtual bool is_txn_control();

  virtual void set_sym_exp(int field_offset, Expression *exp);
};

class SymCommit : public SymStmt {
public:
  SymCommit() : SymStmt(Sym_NULL) {
    set_sql_template("commit");
  }

  void print();

  virtual bool is_txn_control();

  virtual void set_sym_exp(int field_offset, Expression *exp);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLIC_STATEMENT_HPP