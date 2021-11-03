#ifndef SHARE_VM_CONCOLIC_PATHCONDITION_HPP
#define SHARE_VM_CONCOLIC_PATHCONDITION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/symbolicExpression.hpp"

#include <vector>

class PathCondition {
  typedef std::vector<SymbolicExpression *> SymExpList;

private:
  SymExpList _sym_exps;

public:
  void add(SymbolicExpression *sym_exp);
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_PATHCONDITION_HPP