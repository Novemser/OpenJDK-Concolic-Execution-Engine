//
// Created by gansen on 5/18/22.
//

#include "arrayInternal.hpp"

void ArrayInternal::store(int index, Expression *exp) {
  guarantee(index >= 0, "Must be >= 0");
  try_resize(index);
  guarantee(index < (int) _element_exprs.size(), "Must be < size");
  _element_exprs[index] = exp;
  if (exp != NULL) {
    exp->inc_ref();
  }
}

Expression *ArrayInternal::load(int index) {
  if (index >= (int) _element_exprs.size()) {
    return NULL;
  }

  return _element_exprs[index];
}

void ArrayInternal::try_resize(int index) {
  if (index >= (int) _element_exprs.size()) {
    _element_exprs.resize(index + 1, NULL);
  }
}

ArrayInternal::~ArrayInternal() {
  for (size_t elem_index = 0; elem_index < _element_exprs.size(); elem_index++) {
    Expression::gc(_element_exprs[elem_index]);
  }
}
