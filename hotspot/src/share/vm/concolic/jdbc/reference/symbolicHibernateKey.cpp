#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/utils.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "symbolicHibernateKey.hpp"

/* =============================================================
 *                      HibernateKeyExpression
 * =============================================================
 */

HibernateKeyExpression::HibernateKeyExpression(oop obj) {
  std::string key_field_name;

  if (obj->klass()->name()->equals("org/hibernate/engine/spi/EntityKey")) {
    key_field_name = "identifier";
    oop persister = OopUtils::obj_field_by_name(obj, "persister", SigName::EntityPersister);
    set_table_name_exps(persister);
  } else {
    key_field_name = "key";
    guarantee(obj->klass()->name()->equals("org/hibernate/engine/spi/CollectionKey"), "should be");

    oop role_obj = OopUtils::obj_field_by_name(obj, "role", SigName::String);
    push_table_name_exp(role_obj);
  }

  oop key_obj = OopUtils::obj_field_by_name(obj, key_field_name, "Ljava/io/Serializable;");
  set_key_exp(key_obj);
}

HibernateKeyExpression::~HibernateKeyExpression() {
  for (TableNameExps::iterator it = table_name_exps.begin(); it != table_name_exps.end(); it++) {
    Expression::gc(*it);
  }
  table_name_exps.clear();
  Expression::gc(key_exp);
}

void HibernateKeyExpression::set_table_name_exps(oop persister) {
  Symbol *persister_klass_name = persister->klass()->name();
  if (persister_klass_name->equals("org/hibernate/persister/entity/SingleTableEntityPersister")) {
    oop table_names_obj = OopUtils::obj_field_by_name(persister, "qualifiedTableNames", SigName::StringArray);
    set_table_name_exps((objArrayOop)table_names_obj);
  } else if (persister_klass_name->equals("org/hibernate/persister/entity/JoinedSubclassEntityPersister")) {
    oop table_names_obj = OopUtils::obj_field_by_name(persister, "naturalOrderTableNames", SigName::StringArray);
    set_table_name_exps((objArrayOop)table_names_obj);
  } else {
    guarantee(persister_klass_name->equals("org/hibernate/persister/entity/UnionSubclassEntityPersister"), "should be");
    oop table_name = OopUtils::obj_field_by_name(persister, "tableName", SigName::String);
    ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name);
    table_name_exp->inc_ref();
    table_name_exps.push_back(table_name_exp);
  }
}

void HibernateKeyExpression::set_table_name_exps(objArrayOop j_string_vector) {
  int string_vector_length = j_string_vector->length();
  for (int i = 0; i < string_vector_length; i++) {
    oop table_name_obj = j_string_vector->obj_at(i);
    push_table_name_exp(table_name_obj);
  }
}

void HibernateKeyExpression::push_table_name_exp(oop table_name_obj) {
  ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name_obj);
  table_name_exp->inc_ref();
  table_name_exps.push_back(table_name_exp);
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
    } else {
      guarantee(key_obj->klass()->name()->equals("java/lang/String"), "should be");
      key_exp = new ConStringSymbolExp(key_obj);
      key_exp->inc_ref();
    }
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

/* =============================================================
 *                      SymHibernateKey
 * =============================================================
 */

bool SymHibernateKey::need_recording = false;
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

bool SymHibernateKey::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    if (std::equal(callee_name.begin(), callee_name.end(), "<init>")) {
      need_recording = false;
      need_symbolize = true;
    } else {
      need_recording = handle.general_check_param_symbolized();
    }
    if (need_recording) {
      handle.general_prepare_param();
    }
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
//      if (!need_symbolize) {
//        bool recording = handle.general_check_param_symbolized();
//        if (recording) {
//          handle.get_callee_method()->print_name(tty);
//          tty->print_cr(" skipped by SymList, need recording %c", recording ? 'Y' : 'N');
//        }
//      }
    } else {
      bool recording = handle.general_check_param_symbolized();
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymHibernateKey, need recording %c. callee_name: %s", recording ? 'Y' : 'N', callee_name.c_str());
    }
  }

  return need_symbolize;
}

void SymHibernateKey::post_init(oop this_obj) {
  SymHibernateKey *this_sym_inst = (SymHibernateKey *)ConcolicMngr::ctx->get_or_alloc_sym_inst(this_obj);
  Expression *exp = new HibernateKeyExpression(this_obj);
  this_sym_inst->set_ref_exp(exp);
}

Expression *SymHibernateKey::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  if (std::equal(callee_name.begin(), callee_name.end(), "<init>")) {
    need_recording = true;
    post_init(handle.get_param<oop>(0));
  }

  if (!need_recording) {
    return NULL;
  }

  Expression *exp = NULL;
  if (handle_method_names.find(handle.get_callee_name()) != handle_method_names.end()) {

    BasicType type = handle.get_result_type();
    oop obj = NULL;

    switch (type) {
      case T_VOID:
        exp = SymbolExpression::get(Sym_VOID);
        break;
      case T_OBJECT:
        obj = handle.get_result<oop>(type);
        if (obj != NULL) {
          if (obj->is_symbolic()) {
            exp = ConcolicMngr::ctx->get_sym_inst(obj)->get_ref_exp();
            SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
            if (exp == NULL) {
              exp = new InstanceSymbolExp(obj);
              sym_inst->set_ref_exp(exp);
            }
          }
        } else {
          exp = SymbolExpression::get(Sym_NULL);
        }
        break;
      case T_BOOLEAN:
      case T_INT:
        exp = new MethodReturnSymbolExp();
        break;
      default:
        tty->print_cr("%c", type2char(type));
        ShouldNotCallThis();
        break;
    }

    ConcolicMngr::record_path_condition(MethodExpression::get_return_pc(
        handle.get_callee_holder_name(), handle.get_callee_name(),
        handle.get_param_list(), exp));
  }
  return exp;
}

void SymHibernateKey::print() {
  tty->print_cr("SymHibernateKey: ");
  _exp->print();
  tty->cr();
}

#endif