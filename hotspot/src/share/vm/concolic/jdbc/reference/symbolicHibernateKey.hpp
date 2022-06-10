#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICKEY_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICKEY_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/utils.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "concolic/fieldTraverser.hpp"

#include <vector>
#include <string>


class HibernateKeyExpression : public Expression {
  typedef std::vector<ConStringSymbolExp*> TableNameExps;

public:
  Expression *table_name_exp;
  Expression *key_exp;

  HibernateKeyExpression(oop obj);
  ~HibernateKeyExpression();

  void print();

protected:
  virtual void serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer);

private:
  void set_key_exp(oop key_obj);
  void set_table_name_exp(objArrayOop j_string_vector);
  void set_table_name_exp(oop persister);
  Expression* get_composite_key_exp(oop obj);
};

class SymHibernateKey : public SymInstance {
public:
  static const char *TYPE_NAME;

  static std::set<std::string> target_class_names;
  static std::set<std::string> init_target_class_names();
  static std::set<std::string> handle_method_names;
  static std::set<std::string> init_handle_method_names();
  static std::map<std::string, bool> skip_method_names;
  static std::map<std::string, bool> init_skip_method_names();

private:
  exp_map_t _exps;
  Expression *_exp;

  static void post_init(oop this_obj);

public:
  SymHibernateKey(sym_rid_t sym_rid);
  SymHibernateKey(sym_rid_t sym_rid, oop obj);
  ~SymHibernateKey();

  inline static bool target(const std::string &class_name) {
      return target_class_names.find(class_name) != target_class_names.end();
  }

  Expression *get_ref_exp() { return _exp; };
  Expression *create_ref_exp(oop obj);
  void set_ref_exp(Expression *exp) {
      Expression::gc(_exp);
      _exp = exp;
      _exp->inc_ref();
  };

  bool need_recursive() { return false; }
  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static void init_register_class(MethodSymbolizer *m_symbolizer);
};


#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICKEY_HPP
