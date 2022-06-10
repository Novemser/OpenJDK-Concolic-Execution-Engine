//
// Created by gansen on 6/7/22.
//

#ifndef JDK8_CDB_SYMBOLICNDFUNC_HPP
#define JDK8_CDB_SYMBOLICNDFUNC_HPP

#ifdef ENABLE_WEBRIDGE
#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"


class SymbolicNDFunc : public SymInstance {
public:
  static const char *TYPE_NAME;

public:
  SymbolicNDFunc(sym_rid_t sym_rid);
  ~SymbolicNDFunc();

  inline static bool target(const std::string &class_name) {
    return class_name == TYPE_NAME;
  }

  Expression *get_ref_exp() {
    ShouldNotCallThis();
  };

  bool need_recursive() { return false; }
  void print();

public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);
  static void init_register_class(MethodSymbolizer *m_symbolizer);
  static void gc();
private:
  static std::vector<Expression*> _exps;
};

#endif
#endif //JDK8_CDB_SYMBOLICNDFUNC_HPP
