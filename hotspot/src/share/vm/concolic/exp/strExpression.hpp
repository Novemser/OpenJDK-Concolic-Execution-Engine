#ifndef SHARE_VM_CONCOLIC_EXP_STREXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_STREXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "concolic/SymbolicOp.hpp"
#include "concolic/defs.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"

#include <map>
#include <stdio.h>
#include <vector>

class OpStrExpression : public Expression {
    std::string _name;
    exp_list_t _param_list;

public:

    OpStrExpression(const std::string &method, exp_list_t &param_list);
    ~OpStrExpression();

public:
    void print();
};

class ConExpression : public Expression {
    static const int EXP_NAME_LENGTH = 32;

private:
    char _str[EXP_NAME_LENGTH];

public:
    ConExpression(jboolean b);
    ConExpression(jbyte c);
    ConExpression(jchar b);
    ConExpression(jint i);
    ConExpression(jshort s);
    ConExpression(jlong l);
    ConExpression(jfloat f);
    ConExpression(jdouble d);

public:
    void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_EXP_STREXPRESSION_HPP