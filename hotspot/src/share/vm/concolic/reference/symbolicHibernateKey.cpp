#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/exp/hibernateKeyExpression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/reference/symbolicHibernateKey.hpp"
#include "concolic/utils.hpp"


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
SymHibernateKey::SymHibernateKey(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid), _exp(new HibernateKeySymbolExp(obj)) {}

SymHibernateKey::~SymHibernateKey() {
    Expression::gc(_exp);
    exp_map_t::iterator iter;
    for (iter = _exps.begin(); iter != _exps.end(); ++iter) {
      Expression::gc(iter->second);
    }
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