//
// Created by gansen on 5/23/22.
//

#include "symbolicString.hpp"
#include "symbolicHttpServletRequest.hpp"
#include "concolic/concolicMngr.hpp"

bool SymbolicHttpServletRequest::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string callee_name = handle.get_callee_name();
  handle.get_result_type();
  if (callee_name.find("getParameter") == std::string::npos) {
    return false;
  }
  return true;
}

Expression *SymbolicHttpServletRequest::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string callee_name = handle.get_callee_name();
  if (callee_name.find("getParameter") == std::string::npos) {
    return NULL;
  }

  oop result = handle.get_result<oop>(T_OBJECT);
  if (result == NULL) {
    return NULL;
  }

  guarantee(result->klass()->name() == vmSymbols::java_lang_String(), "Should be string");
  SymString *sym_res_str = (SymString *) ConcolicMngr::ctx->alloc_sym_inst(result);
  result->set_sym_rid(sym_res_str->get_sym_rid());
  tty->print_cr("[SymbolicServletReq] returning symbolic result:");
  sym_res_str->print();
  return sym_res_str->get_ref_exp();
}

Expression *SymbolicHttpServletRequest::get_exp_of(oop obj) {
  ShouldNotCallThis();
  return NULL;
}

void SymbolicHttpServletRequest::init_register_class(MethodSymbolizer *m_symbolizer) {
  std::vector<std::string> types;
  types.push_back("javax/servlet/http/HttpServletRequest");
  types.push_back("javax/servlet/ServletRequestWrapper");
  types.push_back("javax/servlet/ServletRequest");
  types.push_back("org/springframework/web/context/request/NativeWebRequest");
  for (size_t types_index = 0; types_index < types.size(); ++types_index) {
    tty->print_cr("Adding servlet handler for class %s", types[types_index].c_str());
    m_symbolizer->add_invoke_helper_methods(types[types_index], invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(types[types_index], finish_method_helper);
  }
}


