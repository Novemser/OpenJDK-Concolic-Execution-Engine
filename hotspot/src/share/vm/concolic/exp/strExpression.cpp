#ifdef ENABLE_CONCOLIC

#include "concolic/exp/strExpression.hpp"
#include "utilities/ostream.hpp"

OpStrExpression::OpStrExpression(const std::string &method, exp_list_t &param_list)
        : _name(method){
    // all exp must be not null

    _param_list.swap(param_list);
    int size = _param_list.size();
    for (int i = 0; i < size; ++i) {
        Expression *exp = _param_list[i];
        if (exp) {
            exp->inc_ref();
        }
    }
}

OpStrExpression::~OpStrExpression() {
    int size = _param_list.size();
    for (int i = 0; i < size; ++i) {
        Expression::gc(_param_list[i]);
    }
}

void OpStrExpression::print() {
    tty->print("(s %s ", _name.c_str());
    int size = _param_list.size();
    for (int i = 0; i < size; ++i) {
        tty->print(",");
        Expression::print_on_maybe_null(_param_list[i]);
    }
    tty->print(" )");
}


ConExpression::ConExpression(jboolean z) {
    int ret = sprintf(_str, "Y_Z_0x%x", z);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jbyte b) {
    int ret = sprintf(_str, "Y_B_0x%x", b);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jchar b) {
    int ret = sprintf(_str, "Y_C_%c", b);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jint i) {
    int ret = sprintf(_str, "Y_I_%d", i);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jshort s) {
    int ret = sprintf(_str, "Y_S_%d", s);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jlong l) {
    int ret = sprintf(_str, "Y_J_%ld", l);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jfloat f) {
    int ret = sprintf(_str, "Y_F_%e", f);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

ConExpression::ConExpression(jdouble d) {
    int ret = sprintf(_str, "Y_D_%le", d);
    assert(ret <= EXP_NAME_LENGTH, "SYM_NAME_LENGTH exceeded!");
}

void ConExpression::print() {
    // TODO: include primiteive type
    tty->indent().print("%s", _str);
}

#endif