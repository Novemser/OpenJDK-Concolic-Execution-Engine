//
// Created by gansen on 5/18/22.
//

#ifndef JDK8_CDB_ARRAYINTERNAL_HPP
#define JDK8_CDB_ARRAYINTERNAL_HPP
#include "oops/oopsHierarchy.hpp"
#include "concolic/exp/expression.hpp"

// representation of an array in cpp form
class ArrayInternal {
private:
  std::vector<Expression* > _element_exprs;
  Expression* _length_expr;
public:
  ArrayInternal(int length) {
    _element_exprs.resize(length);
  }
  void store(int index, Expression* exp);
  Expression* load(int index);
};


#endif //JDK8_CDB_ARRAYINTERNAL_HPP
