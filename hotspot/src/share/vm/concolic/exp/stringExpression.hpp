#ifndef SHARE_VM_CONCOLIC_EXP_STRINGEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_STRINGEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "concolic/SymbolicOp.hpp"
#include "concolic/defs.hpp"
#include "oops/oop.inline.hpp"
#include "utilities/debug.hpp"
#include "utilities/ostream.hpp"
#include "symbolExpression.hpp"

#include <map>
#include <stdio.h>
#include <vector>

class OpStrExpression : public Expression {
    std::string _name;
    exp_list_t _param_list;

public:

    OpStrExpression(const std::string &method, exp_list_t &param_list);
    OpStrExpression(const std::string &method, Expression* exp);
    ~OpStrExpression();

public:
    void print();

    static OpStrExpression* to_string(Expression* exp);
    bool is_op_str_expression() { return true; }
    std::string &get_name() { return _name; }
};

class StringSymbolExp : public SymbolExpression {
public:
  StringSymbolExp(sym_rid_t sym_rid);
};


class ConStringSymbolExp : public SymbolExpression {
public:
    ConStringSymbolExp(oop obj);
    ConStringSymbolExp(const std::string& str);
};

#endif

#endif // SHARE_VM_CONCOLIC_EXP_STRINGEXPRESSION_HPP