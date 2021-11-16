#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"

SymString::SymString(sym_oid_t sym_oid) : SymInstance(sym_oid) {}

SymString::~SymString() {}

#endif