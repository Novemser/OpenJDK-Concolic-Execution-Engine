//
// Created by gansen on 5/18/22.
//

#include "arrayInternal.hpp"

void ArrayInternal::store(int index, Expression *exp) {
  assert(index >= 0, "Must be >= 0");
  if (index >= _element_exprs.size()) {
    _element_exprs.resize(index);
  }
  _element_exprs[index] = exp;
}

Expression *ArrayInternal::load(int index) {
  if (index >= _element_exprs.size()) {
    return NULL;
  }

  return _element_exprs[index];
}
