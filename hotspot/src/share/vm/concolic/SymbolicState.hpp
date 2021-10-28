#ifndef SHARE_VM_CONCOLIC_SYMBOLICSTATE_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICSTATE_HPP

#ifdef ENABLE_CONCOLIC

#include "runtime/handles.hpp"

typedef ulong sym_id_t;

class SymbolicState {
private:
  sym_id_t _sym_id;
  Handle _handle;

  char sym_name[8];

public:
  SymbolicState(sym_id_t sym_id, Handle handle) {
    _sym_id = sym_id;
    _handle = handle;
  
	  handle()->set_handle(handle.raw_value());
		set_sym_name();
	}

  ~SymbolicState() {}

  void set_sym_name() { sprintf(sym_name, "SYM_%ld", _sym_id); }

  char *get_sym_name() { return sym_name; }
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_SYMBOLICSTATE_HPP