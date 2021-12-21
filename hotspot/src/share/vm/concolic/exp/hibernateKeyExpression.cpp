#ifdef ENABLE_CONCOLIC

#include "hibernateKeyExpression.hpp"
#include "concolic/utils.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "concolic/concolicMngr.hpp"


HibernateKeySymbolExp::HibernateKeySymbolExp(oop obj) {
  std::string key_field_name;

  // tty->print_cr("HibernateKeySymbolExp::::::::::::::::::::::::::::::::::::::::::::::::");

  if (obj->klass()->name()->equals("org/hibernate/engine/spi/EntityKey")) {
    key_field_name = "identifier";
    // tty->print_cr("---------- persister ----------");
    oop persister = OopUtils::obj_field_by_name(obj, "persister", "Lorg/hibernate/persister/entity/EntityPersister;");

    // tty->print_cr("---------- table names ----------");
    Symbol *persister_klass_name = persister->klass()->name();
    if (persister_klass_name->equals("org/hibernate/persister/entity/SingleTableEntityPersister")) {
      objArrayOop table_names_obj =
          (objArrayOop)OopUtils::obj_field_by_name(persister, "qualifiedTableNames","[Ljava/lang/String;");
      int table_names_length = table_names_obj->length();
      for (int i = 0; i < table_names_length; i++) {
        oop table_name = table_names_obj->obj_at(i);
        // const char *c_table_name = OopUtils::java_string_to_c(table_name);
        // tty->print("%s ", c_table_name);
        ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name);
        table_name_exp->inc_ref();
        table_name_exps.push_back(table_name_exp);
      }
      // tty->cr();
    } else if (persister_klass_name->equals("org/hibernate/persister/entity/JoinedSubclassEntityPersister")) {
      objArrayOop table_names_obj =
          (objArrayOop)OopUtils::obj_field_by_name(persister,"naturalOrderTableNames","[Ljava/lang/String;");
      int table_names_length = table_names_obj->length();
      for (int i = 0; i < table_names_length; i++) {
        oop table_name = table_names_obj->obj_at(i);
        // const char *c_table_name = OopUtils::java_string_to_c(table_name);
        // tty->print("%s ", c_table_name);
        ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name);
        table_name_exp->inc_ref();
        table_name_exps.push_back(table_name_exp);
      }
      // tty->cr();
    } else {
      assert(persister_klass_name->equals("org/hibernate/persister/entity/UnionSubclassEntityPersister"), "should be");
      oop table_name = OopUtils::obj_field_by_name(persister, "tableName", "Ljava/lang/String;");
      // const char *c_table_name = OopUtils::java_string_to_c(tableName);
      // tty->print("%s ", c_table_name);
      ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(table_name);
      table_name_exp->inc_ref();
      table_name_exps.push_back(table_name_exp);
    }
  } else {
    key_field_name = "key";
    assert(obj->klass()->name()->equals("org/hibernate/engine/spi/CollectionKey"), "should be");

    // tty->print_cr("---------- role ----------");
    oop role = OopUtils::obj_field_by_name(obj, "role", "Ljava/lang/String;");
    // const char *c_role = OopUtils::java_string_to_c(role);
    // tty->print_cr("%s ", c_role);
    ConStringSymbolExp *table_name_exp = new ConStringSymbolExp(role);
    table_name_exp->inc_ref();
    table_name_exps.push_back(table_name_exp);
  }

  // tty->print_cr("---------- identifier/key ----------");
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
    // tty->print("key: %d ", key);
  }
  // tty->print("key exp: ");
  // key_exp->print();
  // tty->cr();

  // tty->print_cr(":::::::::::::::::::::::::::::::::::::::::::::::::");
}

HibernateKeySymbolExp::~HibernateKeySymbolExp() {
  for (TableNameExps::iterator it = table_name_exps.begin(); it != table_name_exps.end(); it++) {
    Expression::gc(*it);
  }
  Expression::gc(key_exp);
}

void HibernateKeySymbolExp::print() {
  tty->print("(k ");
  for (TableNameExps::iterator it = table_name_exps.begin(); it != table_name_exps.end(); it++) {
    (*it)->print();
    tty->print(" ");
  }
  key_exp->print();
  tty->print(")");
}


#endif