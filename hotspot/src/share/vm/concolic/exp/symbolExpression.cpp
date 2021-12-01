#ifdef ENABLE_CONCOLIC

#include "concolic/exp/symbolExpression.hpp"
#include "utilities/ostream.hpp"

char SymbolExpression::str_buf[SymbolExpression::BUF_SIZE];


SymbolExpression *SymbolExpression::shared_exp[number_of_builtin_syms];

void SymbolExpression::init_single_instances() {
  shared_exp[Sym_NULL] = new SymbolExpression("NULL", 4);
  shared_exp[Sym_NULL]->inc_ref();

  shared_exp[Sym_VOID] = new SymbolExpression("VOID", 4);
  shared_exp[Sym_VOID]->inc_ref();
}

void SymbolExpression::set(const char *buf, int length) {
  assert(length <= BUF_SIZE, "BUF_SIZE exceeded!");
  _str = std::string(buf, length);
}

void SymbolExpression::print() { tty->indent().print("%s", _str.c_str()); }

InstanceSymbolExp::InstanceSymbolExp(oop obj) {
  ResourceMark rm;
  const char *class_name = obj->klass()->print_value_string();
  int length = sprintf(str_buf, "S%c_%lu_%s", type2char(T_OBJECT), obj->get_sym_rid(), class_name);
  set(str_buf, length);
}


FieldSymbolExp::FieldSymbolExp(sym_rid_t sym_rid, int field_index,
                               BasicType type) {
  int length =
      sprintf(str_buf, "S%c_%lu.%d", type2char(type), sym_rid, field_index);

  set(str_buf, length);
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