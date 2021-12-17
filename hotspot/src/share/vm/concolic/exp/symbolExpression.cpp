#ifdef ENABLE_CONCOLIC

#include "concolic/exp/symbolExpression.hpp"
#include "concolic/utils.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "runtime/handles.hpp"
#include "precompiled/precompiled.hpp"
#include "utilities/ostream.hpp"

char SymbolExpression::str_buf[SymbolExpression::BUF_SIZE];
char SymbolExpression::temp_buf[SymbolExpression::BUF_SIZE];


SymbolExpression *SymbolExpression::shared_exp[number_of_builtin_syms];

void SymbolExpression::init_single_instances() {
  shared_exp[Sym_NULL] = new SymbolExpression("N", 1);
  shared_exp[Sym_NULL]->inc_ref();

  shared_exp[Sym_VOID] = new SymbolExpression("V", 1);
  shared_exp[Sym_VOID]->inc_ref();
}

void SymbolExpression::set(const char *buf, int length) {
  assert(length <= BUF_SIZE, "BUF_SIZE exceeded!");
  _str = std::string(buf, length);
}

void SymbolExpression::set_head(stringStream &ss, char main_type, BasicType class_type, oop obj) {
  ss.print("%c_%c", main_type, type2char(class_type));
  if (obj != NULL) {
    obj->klass()->print_value_on(&ss);
  }
  ss.print("_");
}

void SymbolExpression::finalize(int length) {
  _str = std::string(str_buf, length);
}

void SymbolExpression::print() { tty->indent().print("%s", _str.c_str()); }

InstanceSymbolExp::InstanceSymbolExp(oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', T_OBJECT, obj);
  ss.print("%lu", obj->get_sym_rid());
  this->finalize(ss.size());
}

KeySymbolExp::KeySymbolExp(oop obj) {
    stringStream ss(str_buf, BUF_SIZE);
    ss.print("K_");

    tty->print_cr("KeySymbolExp::::::::::::::::::::::::::");
    tty->print_cr("---------- persister ----------");
    oop persister = OopUtils::obj_field_by_name(obj, "persister", "Lorg/hibernate/persister/entity/EntityPersister;");
    tty->print_cr("---------- table names ----------");
    Symbol *persister_klass_name = persister->klass()->name();
    if (persister_klass_name->equals("org/hibernate/persister/entity/SingleTableEntityPersister")) {
        objArrayOop table_names =
                (objArrayOop)OopUtils::obj_field_by_name(persister, "qualifiedTableNames","[Ljava/lang/String;");
        int table_names_length = table_names->length();
        for (int i = 0; i < table_names_length; i++) {
            oop table_name = table_names->obj_at(i);
            const char *c_table_name = OopUtils::java_string_to_c(table_name);
            tty->print("%s ", c_table_name);
            ss.print("%s___", c_table_name);
        }
        tty->cr();
    } else if (persister_klass_name->equals("org/hibernate/persister/entity/JoinedSubclassEntityPersister")) {
        objArrayOop table_names =
                (objArrayOop)OopUtils::obj_field_by_name(persister,"naturalOrderTableNames","[Ljava/lang/String;");
        int table_names_length = table_names->length();
        for (int i = 0; i < table_names_length; i++) {
            oop table_name = table_names->obj_at(i);
            const char *c_table_name = OopUtils::java_string_to_c(table_name);
            tty->print("%s ", c_table_name);
            ss.print("%s___", c_table_name);
        }
        tty->cr();
    } else {
        assert(persister_klass_name->equals("org/hibernate/persister/entity/UnionSubclassEntityPersister"), "should be");
        oop tableName = OopUtils::obj_field_by_name(persister, "tableName", "Ljava/lang/String;");
        const char *c_table_name = OopUtils::java_string_to_c(tableName);
        tty->print("%s ", c_table_name);
        ss.print("%s___", c_table_name);
    }
    tty->print_cr("---------- identifier ----------");
    oop identifier = OopUtils::obj_field_by_name(obj, "identifier", "Ljava/io/Serializable;");
    assert(identifier->klass()->name()->equals("Ljava/lang/Long"), "should be");
    long identifier_value = OopUtils::long_field_by_name(identifier, "value", "J");
    tty->print_cr("identifier: %ld", identifier_value);
    ss.print("%ld", identifier_value);

    tty->print_cr(":::::::::::::::::::::::::::");

    ss.print("_%lu", obj->get_sym_rid());
    this->finalize(ss.size());
}

FieldSymbolExp::FieldSymbolExp(sym_rid_t sym_rid, int field_index,
                               BasicType type) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type);
  ss.print("%lu_field%d", sym_rid, field_index);
  this->finalize(ss.size());
}

ArraySymbolExp::ArraySymbolExp(sym_rid_t sym_arr_oid, int version,
                               BasicType type) {
  int length =
      sprintf(str_buf, "A%c_%lu-%d", type2char(type), sym_arr_oid, version);
  set(str_buf, length);
}

ArrayLengthExp::ArrayLengthExp(sym_rid_t sym_arr_oid, BasicType type) {
  int length = sprintf(str_buf, "A%c_%lu.length", type2char(type), sym_arr_oid);
  set(str_buf, length);
}

ElementSymbolExp::ElementSymbolExp(sym_rid_t sym_arr_oid, int version,
                                   int load_count, BasicType type) {
  int length = sprintf(str_buf, "E%c_%lu-%d-%d", type2char(type), sym_arr_oid,
                       version, load_count);
  set(str_buf, length);
}

#endif