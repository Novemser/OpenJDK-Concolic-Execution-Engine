#ifdef ENABLE_CONCOLIC

#include "keyExpression.hpp"
#include "concolic/utils.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "concolic/concolicMngr.hpp"


KeySymbolExp::KeySymbolExp(oop obj) {
  std::string key_field_name;

  // tty->print_cr("KeySymbolExp::::::::::::::::::::::::::::::::::::::::::::::::");

  if (obj->klass()->name()->equals("org/hibernate/engine/spi/EntityKey")) {
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
        const char *c_table_name = OopUtils::java_string_to_c(table_name);
        // tty->print("%s ", c_table_name);
        table_names.push_back(std::string(c_table_name));
      }
      // tty->cr();
    } else if (persister_klass_name->equals("org/hibernate/persister/entity/JoinedSubclassEntityPersister")) {
      objArrayOop table_names_obj =
          (objArrayOop)OopUtils::obj_field_by_name(persister,"naturalOrderTableNames","[Ljava/lang/String;");
      int table_names_length = table_names_obj->length();
      for (int i = 0; i < table_names_length; i++) {
        oop table_name = table_names_obj->obj_at(i);
        const char *c_table_name = OopUtils::java_string_to_c(table_name);
        // tty->print("%s ", c_table_name);
        table_names.push_back(std::string(c_table_name));
      }
      // tty->cr();
    } else {
      assert(persister_klass_name->equals("org/hibernate/persister/entity/UnionSubclassEntityPersister"), "should be");
      oop tableName = OopUtils::obj_field_by_name(persister, "tableName", "Ljava/lang/String;");
      const char *c_table_name = OopUtils::java_string_to_c(tableName);
      // tty->print("%s ", c_table_name);
      table_names.push_back(std::string(c_table_name));
    }

    key_field_name = "identifier";
  } else {
    assert(obj->klass()->name()->equals("org/hibernate/engine/spi/CollectionKey"), "should be");

    // tty->print_cr("---------- role ----------");
    oop role = OopUtils::obj_field_by_name(obj, "role", "Ljava/lang/String;");
    const char *c_role = OopUtils::java_string_to_c(role);
    // tty->print_cr("%s ", c_role);
    table_names.push_back(std::string(c_role));

    key_field_name = "key";
  }

  // tty->print_cr("---------- identifier/key ----------");
  fieldDescriptor fd;
  oop key_obj = OopUtils::obj_field_by_name(obj, key_field_name, "Ljava/io/Serializable;");
  assert(key_obj->klass()->name()->equals("Ljava/lang/Long"), "should be");
  OopUtils::get_fd_by_name(key_obj, "value", "J", fd);
  long key = key_obj->long_field(fd.offset());
  if (key_obj->is_symbolic()) {
    SymInstance *key_sym_inst = ConcolicMngr::ctx->get_sym_inst(key_obj);
    key_exp = key_sym_inst->get(fd.offset());
    assert(key_exp, "should be");
    key_exp->inc_ref();
  } else {
    key_exp = new ConExpression(key);
    key_exp->inc_ref();
  }
  // tty->print("key value: %ld    key exp: ", key);
  // key_exp->print();
  // tty->cr();

  // tty->print_cr(":::::::::::::::::::::::::::::::::::::::::::::::::");
}

KeySymbolExp::~KeySymbolExp() {
  Expression::gc(key_exp);
}

void KeySymbolExp::print() {
  tty->print("(K ");
  for (TableNames::iterator it = table_names.begin(); it != table_names.end(); it++) {
    tty->print("%s ", it->c_str());
  }
  key_exp->print();
  tty->print(")");
}


#endif