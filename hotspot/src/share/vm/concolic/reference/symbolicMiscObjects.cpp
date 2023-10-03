#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicEmailValidator.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"

#include "symbolicMiscObjects.h"

const char *SymbolicMiscObjects::TYPE_NAME = "net/sf/jsqlparser/parser/CCJSqlParser";

void SymbolicMiscObjects::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymbolicMiscObjects::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymbolicMiscObjects::TYPE_NAME, finish_method_helper);
}

bool SymbolicMiscObjects::invoke_method_helper(MethodSymbolizerHandle &handle) {
    return true;
}

Expression *
SymbolicMiscObjects::finish_method_helper(MethodSymbolizerHandle &handle) {
  return NULL;
}

#endif