//
// Created by gansen on 5/23/22.
//

#ifndef JDK8_CDB_SYMBOLICHTTPSERVLETREQUEST_HPP
#define JDK8_CDB_SYMBOLICHTTPSERVLETREQUEST_HPP
#ifdef ENABLE_CONCOLIC
#ifdef ENABLE_WEBRIDGE

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymbolicHttpServletRequest : public SymInstance {
public:
  static bool need_recording;

  static bool invoke_method_helper(MethodSymbolizerHandle &handle);

  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static Expression *get_exp_of(oop obj);

  static void init_register_class(MethodSymbolizer* m_symbolizer);

};
#endif
#endif
#endif //JDK8_CDB_SYMBOLICHTTPSERVLETREQUEST_HPP
