#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/utils.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "symbolicHibernateKey.hpp"

/*
 * HibernateKeyExpression:
 */
const OopUtils::NameSignaturePair HibernateKeyExpression::PERSISTER = std::make_pair("persister", "Lorg/hibernate/persister/entity/EntityPersister;");
const OopUtils::NameSignaturePair HibernateKeyExpression::SINGLE_TABLE_NAMES = std::make_pair("qualifiedTableNames","[Ljava/lang/String;");
const OopUtils::NameSignaturePair HibernateKeyExpression::JOINED_TABLE_NAMES = std::make_pair("naturalOrderTableNames","[Ljava/lang/String;");
const OopUtils::NameSignaturePair HibernateKeyExpression::UNION_TABLE = std::make_pair("tableName", "Ljava/lang/String;");
const OopUtils::NameSignaturePair HibernateKeyExpression::ROLE = std::make_pair("role", "Ljava/lang/String;");

HibernateKeyExpression::HibernateKeyExpression(oop obj) {
  std::string key_field_name;

  if (obj->klass()->name()->equals("org/hibernate/engine/spi/EntityKey")) {
    key_field_name = "identifier";
    oop persister = OopUtils::obj_field_by_name(obj, PERSISTER);

    Symbol *persister_klass_name = persister->klass()->name();
    if (persister_klass_name->equals("org/hibernate/persister/entity/SingleTableEntityPersister")) {
      objArrayOop table_names_obj =
          (objArrayOop)OopUtils::obj_field_by_name(persister, SINGLE_TABLE_NAMES);
      set_table_name_exps(table_names_obj);
    } else if (persister_klass_name->equals("org/hibernate/persister/entity/JoinedSubclassEntityPersister")) {
      objArrayOop table_names_obj =
          (objArrayOop)OopUtils::obj_field_by_name(persister,JOINED_TABLE_NAMES);
      set_table_name_exps(table_names_obj);
    } else {
      assert(persister_klass_name->equals("org/hibernate/persister/entity/UnionSubclassEntityPersister"), "should be");
      oop table_name = OopUtils::obj_field_by_name(persister, UNION_TABLE);
      ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name);
      table_name_exp->inc_ref();
      table_name_exps.push_back(table_name_exp);
    }
  } else {
    key_field_name = "key";
    assert(obj->klass()->name()->equals("org/hibernate/engine/spi/CollectionKey"), "should be");

    oop role = OopUtils::obj_field_by_name(obj, ROLE);
    ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(role);
    table_name_exp->inc_ref();
    table_name_exps.push_back(table_name_exp);
  }

  oop key_obj = OopUtils::obj_field_by_name(obj, key_field_name, "Ljava/io/Serializable;");
  if (key_obj->is_symbolic()) {
    SymInstance *key_sym_inst = ConcolicMngr::ctx->get_sym_inst(key_obj);
    key_exp = key_sym_inst->get_ref_exp();
    assert(key_exp, "should be");
    key_exp->inc_ref();
  } else {
    assert(key_obj->klass()->name()->equals("Ljava/lang/Long"), "should be");
    fieldDescriptor fd;
    OopUtils::get_fd_by_name(key_obj, "value", "J", fd);
    long key = key_obj->long_field(fd.offset());
    key_exp = new ConExpression(key);
    key_exp->inc_ref();
  }
}

HibernateKeyExpression::~HibernateKeyExpression() {
  for (TableNameExps::iterator it = table_name_exps.begin(); it != table_name_exps.end(); it++) {
    Expression::gc(*it);
  }
  table_name_exps.clear();
  Expression::gc(key_exp);
}

void HibernateKeyExpression::set_table_name_exps(objArrayOop j_string_vector) {
  int string_vector_length = j_string_vector->length();
  for (int i = 0; i < string_vector_length; i++) {
    oop table_name = j_string_vector->obj_at(i);
    // const char *c_table_name = OopUtils::java_string_to_c(table_name);
    // tty->print("%s ", c_table_name);
    ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name);
    table_name_exp->inc_ref();
    table_name_exps.push_back(table_name_exp);
  }
}

void HibernateKeyExpression::print() {
  tty->print("(k ");
  for (TableNameExps::iterator it = table_name_exps.begin(); it != table_name_exps.end(); it++) {
    (*it)->print();
    tty->print(" ");
  }
  key_exp->print();
  tty->print(")");
}

/*
 * SymHibernateKey:
 */

bool SymHibernateKey::need_recording = false;
std::set<std::string> SymHibernateKey::target_class_names = init_target_class_names();

std::set<std::string> SymHibernateKey::init_target_class_names() {
    std::set<std::string> set;
    set.insert("org/hibernate/engine/spi/EntityKey");
    set.insert("org/hibernate/engine/spi/CollectionKey");
    return set;
}

method_set_t SymHibernateKey::symbolized_methods = init_symbolized_methods();

method_set_t SymHibernateKey::init_symbolized_methods() {
    method_set_t m_set;
    return m_set;
}

SymHibernateKey::SymHibernateKey(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}
SymHibernateKey::SymHibernateKey(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid), _exp(new HibernateKeyExpression(obj)) {}

SymHibernateKey::~SymHibernateKey() {
    Expression::gc(_exp);
    exp_map_t::iterator iter;
    for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
      Expression::gc(iter->second);
    }
    _exps.clear();
}

Expression *SymHibernateKey::get(int field_offset) {
  exp_map_t::iterator iter = _exps.find(field_offset);
  return iter == _exps.end() ? NULL : iter->second;
}

void SymHibernateKey::init_sym_exp(int field_offset, Expression *exp) {
  exp->inc_ref();
  _exps[field_offset] = exp;
}

void SymHibernateKey::set_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  exp_map_t::iterator iter = _exps.find(field_offset);
  if (iter != _exps.end()) {
    Expression::gc(iter->second);
    if (exp) {
      exp->inc_ref();
      iter->second = exp;
    } else {
      _exps.erase(iter);
    }
  } else if (exp) {
    exp->inc_ref();
    _exps[field_offset] = exp;
  }
}

bool SymHibernateKey::invoke_method_helper(MethodSymbolizerHandle &handle) {
    const std::string &callee_name = handle.get_callee_name();
    bool need_symbolize = false;
    need_symbolize = false;
    if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
        need_symbolize = true;
        handle.get_callee_method()->print_name(tty);
//        tty->print_cr(" handled by SymHibernateKey");
    } else {
        handle.get_callee_method()->print_name(tty);
//        tty->print_cr(" unhandled by SymHibernateKey");
    }

    return need_symbolize;
}

Expression *SymHibernateKey::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (need_recording) {
    handle.get_callee_method()->print_name(tty);
    tty->print_cr(" handled done by SymHibernateKey, return %c", type2char(handle.get_result_type()));
    return MethodSymbolizer::finish_method_helper(handle);
  } else {
    return NULL;
  }
}

void SymHibernateKey::print() {
    tty->print_cr("SymHibernateKey: ");
    _exp->print();
    tty->cr();
}

#endif