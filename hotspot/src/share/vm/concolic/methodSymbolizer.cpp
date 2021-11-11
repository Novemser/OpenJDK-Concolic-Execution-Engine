#ifdef ENABLE_CONCOLIC

#include "concolic/methodSymbolizer.hpp"
#include "memory/resourceArea.hpp"
#include "utilities/ostream.hpp"

void MethodSymbolizer::invoke_method(Method *method) {
  ResourceMark rm;
  tty->print_cr(CL_RED "======================================================="
                       "==========" CNONE);
	if (method->method_holder()->name()->equals("Example")) {
		if (method->name()->equals("func")) {
			
		}
	}
  tty->print_cr(CL_RED "======================================================="
                       "==========" CNONE);
}

#endif