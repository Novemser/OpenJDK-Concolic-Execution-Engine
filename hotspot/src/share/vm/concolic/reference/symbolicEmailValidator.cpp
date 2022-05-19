#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicEmailValidator.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"

const char *SymEmailValidator::TYPE_NAME = "org/apache/commons/validator/routines/EmailValidator";

void SymEmailValidator::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymEmailValidator::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymEmailValidator::TYPE_NAME, finish_method_helper);
}

bool SymEmailValidator::invoke_method_helper(MethodSymbolizerHandle &handle) {
    const std::string &callee_name = handle.get_callee_name();
    bool need_symbolize = false;
    if (callee_name == "isValid"){
        need_symbolize = true;
    }
    return need_symbolize;
}

Expression *
SymEmailValidator::finish_method_helper(MethodSymbolizerHandle &handle) {
  return NULL;
}
#endif