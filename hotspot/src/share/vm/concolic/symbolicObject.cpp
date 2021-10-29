#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void SymbolicObject::symbolize(Handle handle) {
    tty->print("SymbolicObject::symbolize!\n");
    handle()->print();

    Klass* klass = handle()->klass();
}


#endif