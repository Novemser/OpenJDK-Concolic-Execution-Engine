#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICEMAILVALIDATOR_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICEMAILVALIDATOR_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/methodSymbolizerHandle.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/methodSymbolizer.hpp"

class SymEmailValidator : public SymInstance {
public:
  static const char *TYPE_NAME;
  static method_set_t handle_method_names;
  static std::map<std::string, bool> skip_method_names;


public:
  static bool invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static void init_register_class(MethodSymbolizer* m_symbolizer);

};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICEMAILVALIDATOR_HPP