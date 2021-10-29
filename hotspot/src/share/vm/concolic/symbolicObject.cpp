#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "concolic/fieldSymbolizer.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void SymbolicObject::symbolize(Handle handle) {
  tty->print("SymbolicObject::symbolize!\n");

  FieldSymbolizer::do_recursive_symbolize(handle());
  FieldSymbolizer::do_recursive_print(handle());
}

#endif