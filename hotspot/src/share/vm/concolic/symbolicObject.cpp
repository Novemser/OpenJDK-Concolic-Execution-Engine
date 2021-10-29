#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void SymbolicObject::init_sym_exp(int field_index) {
  SymbolicExpression *sym_exp =
      new SymbolicExpression(this->get_sym_name(), field_index);

  sym_exp_store.insert(std::make_pair(field_index, sym_exp));
}

#endif