#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICKEY_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICKEY_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"

class SymKey : public SymInstance {
public:
    static const char *TYPE_NAME;
    static method_set_t symbolized_methods;
private:
    Expression *_exp;

public:
    SymKey(sym_rid_t sym_rid);
    SymKey(sym_rid_t sym_rid, oop obj);
    ~SymKey();

    inline static bool target(const std::string &class_name) {
        return class_name == TYPE_NAME;
    }

    Expression *get_ref_exp() { return _exp; };
    void set_ref_exp(Expression *exp) {
        Expression::gc(_exp);
        _exp = exp;
        _exp->inc_ref();
    };

    bool need_recursive() { return false; }
    void print();

public:
    static bool invoke_method_helper(MethodSymbolizerHandle &handle);
    static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
    static Expression *get_exp_of(oop obj);

private:
    static method_set_t init_symbolized_methods();
};


#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICKEY_HPP
