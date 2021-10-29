#ifndef SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/symbolicExpression.hpp"
#include "memory/resourceArea.hpp"
#include "oops/instanceKlass.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "runtime/handles.hpp"
#include "utilities/debug.hpp"

#include <map>
#include <stdio.h>

class SymbolicObject {
  static const int SYM_NAME_LENGTH = 8;
  typedef std::map<int, SymbolicExpression *> ExpStore;

private:
  sym_oid_t _sym_oid;
  char sym_name[SYM_NAME_LENGTH];

public:
  SymbolicObject(sym_oid_t sym_oid) {
    _sym_oid = sym_oid;

    set_sym_name();
  }

  void set_sym_name() {
    int ret = sprintf(sym_name, "S_%ld", _sym_oid);
    assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
  }

  char *get_sym_name() { return sym_name; }

  void symbolize(Handle handle);
  static void do_symbolize(oop obj, int depth);
};

class FieldSymbolizer : public FieldClosure {
  typedef bool (FieldSymbolizer::*fieldHandler)(fieldDescriptor *fd, oop obj) ;

private:
  int _depth;
  oop _obj;
  fieldHandler _field_handler;

public:
  FieldSymbolizer(oop obj, int depth, fieldHandler field_handler)
      : _obj(obj), _depth(depth), _field_handler(field_handler) {}
  void do_field_helper();
  void do_field(fieldDescriptor *fd);

  static void do_symbolize(oop obj);
  bool print_field(fieldDescriptor *fd, oop obj);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_SYMBOLICOBJECT_HPP