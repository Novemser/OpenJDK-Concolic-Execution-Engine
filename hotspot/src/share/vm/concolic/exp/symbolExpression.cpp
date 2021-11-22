#ifdef ENABLE_CONCOLIC

#include "concolic/exp/symbolExpression.hpp"
#include "utilities/ostream.hpp"

sym_rid_t SymbolExpression::sym_method_count = 0;

SymbolExpression::SymbolExpression(sym_rid_t sym_rid, int field_index,
                                   BasicType type) {
  int ret;
  if (field_index == NULL_INDEX) {
    ret = sprintf(_str, "S%c_%lu", type2char(type), sym_rid);
  } else if (field_index == FIELD_INDEX_ARRAY_LENGTH) {
    ret = sprintf(_str, "A%c_%lu.length", type2char(type), sym_rid);
  } else {
    ret = sprintf(_str, "S%c_%lu.%d", type2char(type), sym_rid, field_index);
  }
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

SymbolExpression::SymbolExpression(sym_rid_t sym_arr_oid, int version,
                                   int load_count, BasicType type) {

  int ret = sprintf(_str, "E%c_%lu-%d-%d", type2char(type), sym_arr_oid,
                    version, load_count);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

SymbolExpression::SymbolExpression() {
  int ret = sprintf(_str, "M_%lu", sym_method_count++);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

SymbolExpression::SymbolExpression(const char *prefix, sym_rid_t id) {
  int ret = sprintf(_str, "%s_%lu", prefix, id);
  assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

SymbolExpression::~SymbolExpression() {}

void SymbolExpression::print() { tty->indent().print("%s", _str); }

#endif