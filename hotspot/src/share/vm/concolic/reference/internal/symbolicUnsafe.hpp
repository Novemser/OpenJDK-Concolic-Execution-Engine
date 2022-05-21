//
// Created by gansen on 5/20/22.
//

#ifndef JDK8_CDB_SYMBOLICUNSAFE_HPP
#define JDK8_CDB_SYMBOLICUNSAFE_HPP
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "concolic/exp/expression.hpp"

class SymbolicUnsafe {
public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static Expression *get_exp_of(oop obj);
  static void init_register_class(MethodSymbolizer *m_symbolizer);
};


#endif //JDK8_CDB_SYMBOLICUNSAFE_HPP
