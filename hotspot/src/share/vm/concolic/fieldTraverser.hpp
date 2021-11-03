#ifndef SHARE_VM_CONCOLIC_FIELDTRAVERSER_HPP
#define SHARE_VM_CONCOLIC_FIELDTRAVERSER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/threadContext.hpp"
#include "memory/resourceArea.hpp"
#include "oops/instanceKlass.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "runtime/handles.hpp"
#include "utilities/debug.hpp"

#include <stdio.h>

class FieldTraverser : public FieldClosure {
private:
  oop _obj;
  int _depth, _target_depth;

public:
  // recursive functions
  void do_recursive();
  void do_once();

protected:
  FieldTraverser(oop obj) : _obj(obj), _depth(1) {}

  virtual bool do_field_helper(fieldDescriptor *fd, oop obj) {}
  virtual bool do_array_element_helper(int index, arrayOop array_obj) {}

  void print_indent();

private:
  void do_field(fieldDescriptor *fd);
  void do_array_element(int index);
  void do_recursive_helper();
  void do_array_elements(FieldTraverser* field_traverser);
};

class FieldSymbolizer : public FieldTraverser {
private:
  ThreadContext &_ctx;

public:
  FieldSymbolizer(oop obj, ThreadContext &ctx)
      : FieldTraverser(obj), _ctx(ctx) {}

protected:
  bool do_field_helper(fieldDescriptor *fd, oop obj);
  bool do_array_element_helper(int index, arrayOop array_obj);
};

class SimpleFieldPrinter : public FieldTraverser {
public:
  SimpleFieldPrinter(oop obj) : FieldTraverser(obj) {}

protected:
  bool do_field_helper(fieldDescriptor *fd, oop obj);
  bool do_array_element_helper(int index, arrayOop array_obj);
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_FIELDTRAVERSER_HPP