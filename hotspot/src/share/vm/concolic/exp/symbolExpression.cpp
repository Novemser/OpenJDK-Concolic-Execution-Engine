#ifdef ENABLE_CONCOLIC

#include "concolic/exp/symbolExpression.hpp"
#include "concolic/utils.hpp"
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

void SymbolExpression::set_head(stringStream &ss, char main_type, BasicType class_type, const char* klass_name) {
  ss.print("%c_%c%s_", main_type, type2char(class_type), klass_name);
}

void SymbolExpression::finalize(int length) {
  _str = std::string(str_buf, length);
}

void SymbolExpression::print() { tty->indent().print("%s", _str.c_str()); }

void SymbolExpression::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("SymbolExpression");
  writer.Key("_java_type");
  writer.String(_type.c_str());
  writer.Key("_exp");
  writer.String(_str.c_str());
}

InstanceSymbolExp::InstanceSymbolExp(oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', T_OBJECT, obj);
  if (obj != NULL) {
    ResourceMark rm;
    _type = obj->klass()->name()->as_C_string();
  }
  ss.print("%lu", obj->get_sym_rid());
  this->finalize(ss.size());
}

PlaceHolderSymbolExp::PlaceHolderSymbolExp(oop obj) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'P', T_OBJECT, obj);
  if (obj != NULL) {
    ResourceMark rm;
    _type = obj->klass()->name()->as_C_string();
  }
  this->finalize(ss.size());
}

FieldSymbolExp::FieldSymbolExp(sym_rid_t sym_rid, int field_index,
                               BasicType type) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type);
  ss.print("%lu_field%d", sym_rid, field_index);
  _type = type2char(type);
  this->finalize(ss.size());
}

FieldSymbolExp::FieldSymbolExp(sym_rid_t sym_rid, std::string name,
                               BasicType type) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type);
  ss.print("field_%s", name.c_str());
  _type = type2char(type);
  this->finalize(ss.size());
}

ConSymbolExp::ConSymbolExp(const char* str, BasicType type) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'Y', type);
  ss.print("%s", str);
  _type = type2char(type);
  this->finalize(ss.size());
}

void ConSymbolExp::serialize_internal(rapidjson::Writer<rapidjson::StringBuffer> &writer) {
  writer.Key("_type");
  writer.String("ConSymbolExp");
  writer.Key("_java_type");
  writer.String(_type.c_str());
  writer.Key("_exp");
  writer.String(_str.c_str());
}


ArraySymbolExp::ArraySymbolExp(sym_rid_t sym_arr_oid, int version,
                               BasicType type) {
  int length =
      sprintf(str_buf, "A%c_%lu-%d", type2char(type), sym_arr_oid, version);
  set(str_buf, length);
  _type = type2char(type);
}

ArrayLengthExp::ArrayLengthExp(sym_rid_t sym_arr_oid, BasicType type) {
  int length = sprintf(str_buf, "A%c_%lu.length", type2char(type), sym_arr_oid);
  set(str_buf, length);
  _type = type2char(T_INT);
}

ElementSymbolExp::ElementSymbolExp(sym_rid_t sym_arr_oid, int version,
                                   int load_count, BasicType type) {
  int length = sprintf(str_buf, "E%c_%lu-%d-%d", type2char(type), sym_arr_oid,
                       version, load_count);
  set(str_buf, length);
  _type = type2char(type);
}


#endif