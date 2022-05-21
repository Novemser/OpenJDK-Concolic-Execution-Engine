//
// Created by gansen on 5/20/22.
//

#include "symbolicUnsafe.hpp"

bool SymbolicUnsafe::invoke_method_helper(MethodSymbolizerHandle &handle) {
//  tty->print_cr("Inside unsafe %s", handle.get_callee_name().c_str());
  return true;
}

Expression *SymbolicUnsafe::finish_method_helper(MethodSymbolizerHandle &handle) {
  return NULL;
}

Expression *SymbolicUnsafe::get_exp_of(oop obj) {
  return NULL;
}

void SymbolicUnsafe::init_register_class(MethodSymbolizer *m_symbolizer) {
  const std::string unsafe_klass_name("sun/misc/Unsafe");
  const std::string sun_instrument("sun/instrument/TransformerManager");
  std::vector<const std::string*> klasses;
  klasses.push_back(&unsafe_klass_name);
  klasses.push_back(&sun_instrument);
  for (size_t index = 0; index < klasses.size(); ++index) {
    m_symbolizer->add_invoke_helper_methods(klasses[index]->c_str(), invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(klasses[index]->c_str(), finish_method_helper);
  }
}
