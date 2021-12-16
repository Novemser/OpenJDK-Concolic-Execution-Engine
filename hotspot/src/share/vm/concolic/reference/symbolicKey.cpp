#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicKey.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

const char *SymKey::TYPE_NAME = "org/hibernate/engine/spi/EntityKey";
//method_set_t SymKey::symbolized_methods = init_symbolized_methods();
//
//method_set_t SymKey::init_symbolized_methods() {
//    method_set_t m_set;
////    m_set.insert("<init>");
//    return m_set;
//}

SymKey::SymKey(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}
SymKey::SymKey(sym_rid_t sym_rid, oop obj) : SymInstance(sym_rid), _exp(new KeySymbolExp(obj)) {}

SymKey::~SymKey() {
    Expression::gc(_exp);
}

Expression *SymKey::get_exp_of(oop obj) {
    assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
    Expression *exp;
    if (obj->is_symbolic()) {
        SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
        exp = sym_inst->get_ref_exp();
        assert(exp != NULL, "NOT NULL");
    } else {
        ResourceMark rm;
        // TODO: check this
        exp = new KeySymbolExp(obj);
    }
    return exp;
}

bool SymKey::invoke_method_helper(MethodSymbolizerHandle &handle) {
    const std::string &callee_name = handle.get_callee_name();
    bool need_symbolize = false;
    if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
        need_symbolize = true;
    } else {
        handle.get_callee_method()->print_name(tty);
        tty->print_cr("unhandled by SymKey:");
    }

    return need_symbolize;
}

Expression *SymKey::finish_method_helper(MethodSymbolizerHandle &handle) {
    const std::string &callee_name = handle.get_callee_name();
    Expression *exp = NULL;
    return exp;
}

void SymKey::print() {
    tty->print_cr("SymKey: ");
    _exp->print();
    tty->print("\n");
}

#endif