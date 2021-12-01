#ifndef SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP
#define SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP

#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_JDBC)

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymResSet;
class ResultSetSymbolExp : public SymbolExpression {
public:
  ResultSetSymbolExp(SymResSet *sym_res_set);
  ResultSetSymbolExp(SymResSet *sym_res_set, const char *col_name);
};
class SymResSet : public SymInstance {
  friend class ResultSetSymbolExp;

public:
  inline static bool target(const std::string &class_name) {
    return target_class_names.find(class_name) != target_class_names.end();
  }

private:
  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> handle_method_names;
  static std::set<std::string> init_handle_method_names();
  static std::set<std::string> skip_method_names;
  static std::set<std::string> init_skip_method_names();

private:
  sym_rid_t _sym_stmt_rid;
  int _sql_id;
  int _row_id;

  Expression *_size_exp;
  Expression* _ref_exp;

public:
  SymResSet(sym_rid_t sym_rid);
  ~SymResSet();

  /**
   * Handle these redandent code
   */
  Expression *get_ref_exp() { return _ref_exp; };

  void set_ref_exp(Expression *exp) {
    if (_ref_exp && _ref_exp->dec_ref()) {
      delete _ref_exp;
    }
    _ref_exp = exp;
    _ref_exp->inc_ref();
  };
public:
  inline void set_stmt_rid(sym_rid_t sym_stmt_rid) {
    _sym_stmt_rid = sym_stmt_rid;
    _sql_id = (int)sym_stmt_rid;
    _size_exp = new ResultSetSymbolExp(this);
    _size_exp->inc_ref();
  }

  inline Expression *get_size_exp() { return _size_exp; }

  inline int get_row_id() { return _row_id; }

  inline void next() { ++_row_id; }

public:
  bool need_recursive() { return false; }

  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
};

#endif // ENABLE_CONCOLIC && CONCOLIC_JDBC
#endif // SHARE_VM_CONCOLIC_JDBC_REFERENCE_SYMBOLICRESULTSET_HPP