#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicInstance.hpp"
#include "symbolicBigDecimal.hpp"

Expression *SymInstance::get_exp_of(oop obj) {
  Expression *exp = NULL;
  Symbol *klass_symbol = obj->klass()->name();
  if (klass_symbol->equals(SymBigDecimal::TYPE_NAME)) {
    exp = SymBigDecimal::get_exp_of(obj);
  } else {
    klass_symbol->print_symbol_on(tty);
    ShouldNotReachHere();
  }
  return exp;
}

#endif // ENABLE_CONCOLIC
