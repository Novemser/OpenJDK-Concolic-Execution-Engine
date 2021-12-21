#ifndef SHARE_VM_CONCOLIC_EXP_KEYEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_EXP_KEYEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "concolic/exp/symbolExpression.hpp"

#include <vector>
#include <string>

class HibernateKeySymbolExp : public Expression {
  typedef std::vector<ConStringSymbolExp*> TableNameExps;

public:
  TableNameExps table_name_exps;
  Expression *key_exp;

  HibernateKeySymbolExp(oop obj);
  ~HibernateKeySymbolExp();

  void print();
};


#endif
#endif //SHARE_VM_CONCOLIC_EXP_KEYEXPRESSION_HPP
