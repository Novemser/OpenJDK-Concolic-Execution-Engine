#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicEmailValidator.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"

#include "symbolicMiscObjects.h"

void SymbolicMiscObjects::init_register_class(MethodSymbolizer *m_symbolizer) {
  std::vector<std::string> types;
  // FIXME: concolic executing into some methods inside these classes triggers JVM crash. Investigate them later.
  types.push_back("com/fasterxml/jackson/core/json/UTF8JsonGenerator");
  types.push_back("com/fasterxml/jackson/databind/ser/BeanPropertyWriter");
  types.push_back("net/sf/jsqlparser/parser/CCJSqlParser");
  types.push_back("freemarker/core/FMParserTokenManager");
  types.push_back("org/apache/ibatis/ognl/OgnlParser");
  types.push_back("org/apache/ibatis/ognl/Ognl");
  types.push_back("org/attoparser/MarkupParser");
  types.push_back("org/thymeleaf/TemplateEngine");
  types.push_back("org/apache/http/impl/execchain/ProtocolExec");
  for (size_t types_index = 0; types_index < types.size(); ++types_index) {
    tty->print_cr("Adding disable concolic handler for class %s", types[types_index].c_str());
    m_symbolizer->add_invoke_helper_methods(types[types_index], invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(types[types_index], finish_method_helper);
  }
}

bool SymbolicMiscObjects::invoke_method_helper(MethodSymbolizerHandle &handle) {
    return true;
}

Expression *
SymbolicMiscObjects::finish_method_helper(MethodSymbolizerHandle &handle) {
  return NULL;
}

#endif