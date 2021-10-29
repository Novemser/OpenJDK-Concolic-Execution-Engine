#ifndef SHARE_VM_CONCOLIC_FIELDSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_FIELDSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/symbolicExpression.hpp"
#include "memory/resourceArea.hpp"
#include "oops/instanceKlass.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "runtime/handles.hpp"
#include "utilities/debug.hpp"

#include <stdio.h>


class FieldSymbolizer : public FieldClosure {
  typedef bool (FieldSymbolizer::*fieldHandler)(fieldDescriptor *fd, oop obj);

private:
  int _depth;
  oop _obj;
  fieldHandler _field_handler;

public:
  FieldSymbolizer(oop obj, fieldHandler field_handler)
      : _obj(obj), _depth(0), _field_handler(field_handler) {}
  
  static void do_recursive_symbolize(oop obj);
  static void do_recursive_print(oop obj);

  bool print_field(fieldDescriptor *fd, oop obj);
  bool symbolize_field(fieldDescriptor *fd, oop obj);

  // recursive functions
  void do_field_helper();
  void do_field(fieldDescriptor *fd);
private:
  void print_indent();
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_FIELDSYMBOLIZER_HPP