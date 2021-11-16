#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"

SymString::SymString(sym_rid_t sym_rid) : SymInstance(sym_rid) {}

SymString::~SymString() {}

#endif