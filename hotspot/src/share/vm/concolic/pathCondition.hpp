#ifndef SHARE_VM_CONCOLIC_PATHCONDITION_HPP
#define SHARE_VM_CONCOLIC_PATHCONDITION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <vector>

class PathCondition {
  struct Condition {
    std::string _code_pos;
    Expression *_exp;

    Condition(std::string code_pos, Expression *exp)
        : _code_pos(code_pos), _exp(exp) {
      exp->inc_ref();
    }

    ~Condition() { Expression::gc(_exp); }
  };

private:
  std::vector<Condition *> _conds;

public:
  PathCondition() {}

  PathCondition(const PathCondition& original) {
    for (size_t index = 0; index < original._conds.size(); ++index) {
      this->_conds.push_back(original._conds[index]);
    }
  }

  void add(Expression *exp);
  void gc();
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_PATHCONDITION_HPP