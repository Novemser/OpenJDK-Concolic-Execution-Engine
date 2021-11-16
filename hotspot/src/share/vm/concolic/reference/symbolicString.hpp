#ifndef SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP
#define SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicInstance.hpp"
#include <string>

class SymString : public SymInstance {
private:
  std::string str;

public:
  SymString(sym_oid_t sym_oid);
  ~SymString();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_REFERENCE_SYMBOLICSTRING_HPP