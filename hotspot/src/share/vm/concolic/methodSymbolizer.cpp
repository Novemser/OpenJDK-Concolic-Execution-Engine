#ifdef ENABLE_CONCOLIC

#include "concolic/methodSymbolizer.hpp"
#include "concolic/concolicMngr.hpp"
#include "memory/resourceArea.hpp"
#include "utilities/ostream.hpp"

void MethodSymbolizer::invoke_method(ZeroFrame *caller_frame) {
  // ResourceMark rm;
  if (caller_frame->is_interpreter_frame()) {
    interpreterState caller_istate =
        caller_frame->as_interpreter_frame()->interpreter_state();
    Method *callee = caller_istate->callee();
    if (callee->method_holder()->name()->equals("Example")) {
			// tty->print_cr("Calling function name: %s", callee->name()->as_C_string());
      if (callee->name()->equals("func")) {
        ConcolicMngr::is_symbolizing_method = true;
      }
    }
  }
}

void MethodSymbolizer::finish_method(ZeroFrame *caller_frame) {
  tty->print_cr(CL_RED "======================" CNONE);
	ConcolicMngr::record_path_condition(new ConExpression(1));
  ConcolicMngr::is_symbolizing_method = false;
}
#endif