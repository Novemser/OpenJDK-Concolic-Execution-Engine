#ifdef ENABLE_CONCOLIC

#include "concolic/exp/stringExpression.hpp"
#include "utilities/ostream.hpp"

void StringExpression::print() { tty->print_cr("\"%s\"", _str.c_str()); }

#endif