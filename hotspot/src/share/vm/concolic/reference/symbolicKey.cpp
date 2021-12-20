#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/exp/keyExpression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/reference/symbolicKey.hpp"
#include "concolic/utils.hpp"


bool SymKey::need_recording = false;
std::set<std::string> SymKey::target_class_names = init_target_class_names();

std::set<std::string> SymKey::init_target_class_names() {
    std::set<std::string> set;
    set.insert("org/hibernate/engine/spi/EntityKey");
    set.insert("org/hibernate/engine/spi/CollectionKey");
    return set;
}

method_set_t SymKey::symbolized_methods = init_symbolized_methods();

method_set_t SymKey::init_symbolized_methods() {
    method_set_t m_set;
    return m_set;
}

SymKey::SymKey(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}
SymKey::SymKey(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid), _exp(new KeySymbolExp(obj)) {}

SymKey::~SymKey() {
    Expression::gc(_exp);
    exp_map_t::iterator iter;
    for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
      Expression::gc(iter->second);
    }
}

Expression *SymKey::get(int field_offset) {
  exp_map_t::iterator iter = _exps.find(field_offset);
  return iter == _exps.end() ? NULL : iter->second;
}

void SymKey::init_sym_exp(int field_offset, Expression *exp) {
  exp->inc_ref();
  _exps[field_offset] = exp;
}

void SymKey::set_sym_exp(int field_offset, Expression *exp) {
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

bool SymKey::invoke_method_helper(MethodSymbolizerHandle &handle) {
    const std::string &callee_name = handle.get_callee_name();
    bool need_symbolize = false;
    need_symbolize = false;
    if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
        need_symbolize = true;
        handle.get_callee_method()->print_name(tty);
//        tty->print_cr(" handled by SymKey");
    } else {
        handle.get_callee_method()->print_name(tty);
//        tty->print_cr(" unhandled by SymKey");
    }

    return need_symbolize;
}

Expression *SymKey::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (need_recording) {
    handle.get_callee_method()->print_name(tty);
    tty->print_cr(" handled done by SymKey, return %c", type2char(handle.get_result_type()));
    return MethodSymbolizer::finish_method_helper(handle);
  } else {
    return NULL;
  }
}

void SymKey::print() {
    tty->print_cr("SymKey: ");
    _exp->print();
    tty->cr();
}

#endif