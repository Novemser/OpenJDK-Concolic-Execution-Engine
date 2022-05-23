//
// Created by gansen on 5/18/22.
//

#ifndef JDK8_CDB_ARRAYINTERNAL_HPP
#define JDK8_CDB_ARRAYINTERNAL_HPP
#ifdef ENABLE_CONCOLIC
#include "oops/oopsHierarchy.hpp"
#include "concolic/exp/expression.hpp"

// representation of an array in cpp form
class ArrayInternal {
private:
  std::vector<Expression* > _element_exprs;

  void try_resize(int index);
public:
  ArrayInternal(int length) {
    guarantee(length > 0, "Array length should be > 0");
    _element_exprs.resize(length + 1, NULL);
  }
  ~ArrayInternal();

  void store(int index, Expression* exp);
  Expression* load(int index);
};

#endif
#endif //JDK8_CDB_ARRAYINTERNAL_HPP
