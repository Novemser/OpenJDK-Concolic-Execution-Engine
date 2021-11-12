#ifdef ENABLE_CONCOLIC

#include "concolic/methodSymbolizer.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "memory/resourceArea.hpp"
#include "utilities/ostream.hpp"

void MethodSymbolizer::invoke_method(ZeroFrame *caller_frame) {
  // ResourceMark rm;
  if (caller_frame->is_interpreter_frame()) {
    interpreterState caller_istate =
        caller_frame->as_interpreter_frame()->interpreter_state();
    Method *callee = caller_istate->callee();
    if (callee->method_holder()->name()->equals("Example")) {
      // tty->print_cr("Calling function name: %s",
      // callee->name()->as_C_string());
      if (callee->name()->equals("func")) {
        int param_length = callee->method_parameters_length();
        _frame = caller_frame;
        ConcolicMngr::is_symbolizing_method = true;
      }
    }
  }
}

void MethodSymbolizer::finish_method(ZeroFrame *caller_frame) {
  assert(caller_frame == _frame, "the same frame");
  tty->print_cr(CL_RED "======================" CNONE);
  ConcolicMngr::record_path_condition(new MethodExpression(
      "Example", "func", _param_list, new ConExpression(1)));
  this->reset();
	ConcolicMngr::is_symbolizing_method = false;
}

void MethodSymbolizer::reset() {
  _frame = NULL;
  _param_list.clear();
}
#endif