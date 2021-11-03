#ifndef SHARE_VM_CONCOLIC_PATHCONDITION_HPP
#define SHARE_VM_CONCOLIC_PATHCONDITION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <vector>

class PathCondition {
  typedef std::vector<Expression *> SymExpList;

private:
  SymExpList _exps;

public:
  void add(Expression *exp);
  void gc();
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_PATHCONDITION_HPP