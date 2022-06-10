#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/utils.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "symbolicHibernateKey.hpp"
#include "concolic/fieldTraverser.hpp"

#include <algorithm>

/* =============================================================
 *                      HibernateKeyExpression
 * =============================================================
 */

HibernateKeyExpression::HibernateKeyExpression(oop obj) {
  std::string key_field_name;

  if (obj->klass()->name()->equals("org/hibernate/engine/spi/EntityKey")) {
    key_field_name = "identifier";
    oop persister = OopUtils::obj_field_by_name(obj, "persister", SigName::EntityPersister);
    set_table_name_exp(persister);
  } else {
    key_field_name = "key";
    guarantee(obj->klass()->name()->equals("org/hibernate/engine/spi/CollectionKey"), "should be");

    oop role_obj = OopUtils::obj_field_by_name(obj, "role", SigName::String);

    ResourceMark rm;
    std::string table_name = OopUtils::java_string_to_c(role_obj);
    std::replace(table_name.begin(), table_name.end(), '.', '/');
    table_name_exp = new ConStringSymbolExp(table_name);
    table_name_exp->inc_ref();
  }

  oop key_obj = OopUtils::obj_field_by_name(obj, key_field_name, "Ljava/io/Serializable;");
  set_key_exp(key_obj);
}

HibernateKeyExpression::~HibernateKeyExpression() {
  Expression::gc(table_name_exp);
  Expression::gc(key_exp);
}

void HibernateKeyExpression::set_table_name_exp(oop persister) {
  Symbol *persister_klass_name = persister->klass()->name();
  if (persister_klass_name->equals("org/hibernate/persister/entity/SingleTableEntityPersister")) {
    oop table_names_obj = OopUtils::obj_field_by_name(persister, "qualifiedTableNames", SigName::StringArray);
    set_table_name_exp((objArrayOop) table_names_obj);
  } else if (persister_klass_name->equals("org/hibernate/persister/entity/JoinedSubclassEntityPersister")) {
    oop table_names_obj = OopUtils::obj_field_by_name(persister, "naturalOrderTableNames", SigName::StringArray);
    set_table_name_exp((objArrayOop) table_names_obj);
  } else {
    guarantee(persister_klass_name->equals("org/hibernate/persister/entity/UnionSubclassEntityPersister"), "should be");
    oop table_name = OopUtils::obj_field_by_name(persister, "tableName", SigName::String);
    table_name_exp = new ConStringSymbolExp(table_name);
    table_name_exp->inc_ref();
  }
}

void HibernateKeyExpression::set_table_name_exp(objArrayOop j_string_vector) {
  std::string table_name = OopUtils::java_string_to_c(j_string_vector->obj_at(0));
  int string_vector_length = j_string_vector->length();
  for (int i = 1; i < string_vector_length; i++) {
    table_name += "_";
    table_name += OopUtils::java_string_to_c(j_string_vector->obj_at(i));
  }
  std::replace(table_name.begin(), table_name.end(), '.', '/');
  table_name_exp = new ConStringSymbolExp(table_name);
  table_name_exp->inc_ref();
}

void HibernateKeyExpression::set_key_exp(oop key_obj) {
  if (key_obj->is_symbolic()) {
    SymInstance *key_sym_inst = ConcolicMngr::ctx->get_sym_inst(key_obj);
    key_exp = key_sym_inst->get_ref_exp();
    guarantee(key_exp, "should be");
    key_exp->inc_ref();
  } else {
    Symbol *key_obj_klass_name = key_obj->klass()->name();
    if (key_obj_klass_name->equals("java/lang/Long")) {
      fieldDescriptor fd;
      OopUtils::get_fd_by_name(key_obj, "value", "J", fd);
      long key = key_obj->long_field(fd.offset());
      key_exp = new ConExpression(key);
      key_exp->inc_ref();
    } else if (key_obj->klass()->name()->equals("java/lang/String")) {
      key_exp = new ConStringSymbolExp(key_obj);
      key_exp->inc_ref();
    } else {
      key_exp = get_composite_key_exp(key_obj);
      key_exp->inc_ref();
    }
  }
}

void HibernateKeyExpression::print() {
  tty->print("(k ");
  table_name_exp->print();
  tty->print(" ");
  key_exp->print();
  tty->print(")");
}

Expression* HibernateKeyExpression::get_composite_key_exp(oop obj) {
  CompositeKeyGenerator key_generator(obj);
  key_generator.do_once();
  return key_generator.get_key_exp();
}

void HibernateKeyExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  ShouldNotCallThis();
}

/* =============================================================
 *                      SymHibernateKey
 * =============================================================
 */

std::set<std::string> SymHibernateKey::target_class_names = init_target_class_names();

std::set<std::string> SymHibernateKey::init_target_class_names() {
  std::set<std::string> set;
  set.insert("org/hibernate/engine/spi/EntityKey");
  set.insert("org/hibernate/engine/spi/CollectionKey");
  return set;
}

std::set<std::string> SymHibernateKey::handle_method_names = init_handle_method_names();

std::set<std::string> SymHibernateKey::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("<init>");
  return set;
}

std::map<std::string, bool> SymHibernateKey::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymHibernateKey::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["generateHashCode"] = true;
  return map;
}

void SymHibernateKey::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}


SymHibernateKey::SymHibernateKey(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}

SymHibernateKey::SymHibernateKey(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid),
                                                               _exp(new HibernateKeyExpression(obj)) {}

SymHibernateKey::~SymHibernateKey() {
  Expression::gc(_exp);
  exp_map_t::iterator iter;
  for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
    Expression::gc(iter->second);
  }
  _exps.clear();
}

Expression *SymHibernateKey::create_ref_exp(oop obj) {
  return new HibernateKeyExpression(obj);
}

bool SymHibernateKey::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (std::equal(callee_name.begin(), callee_name.end(), "<init>")) {
    need_symbolize = true;
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
    } else {
      bool recording = handle.general_check_param_symbolized();
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymHibernateKey, need recording %c. callee_name: %s", recording ? 'Y' : 'N',
                    callee_name.c_str());
    }
  }

  return need_symbolize;
}

void SymHibernateKey::post_init(oop this_obj) {
  SymHibernateKey *this_sym_inst = (SymHibernateKey *) ConcolicMngr::ctx->get_or_alloc_sym_inst(this_obj);
  Expression *exp = new HibernateKeyExpression(this_obj);
  this_sym_inst->set_ref_exp(exp);
}

Expression *SymHibernateKey::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  if (std::equal(callee_name.begin(), callee_name.end(), "<init>")) {
    post_init(handle.get_param<oop>(0));
  }
  return NULL;
}

void SymHibernateKey::print() {
  tty->print_cr("SymHibernateKey: ");
  _exp->print();
  tty->cr();
}

#endif