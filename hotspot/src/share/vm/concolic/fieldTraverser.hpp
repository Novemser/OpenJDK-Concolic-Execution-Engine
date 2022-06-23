#ifndef SHARE_VM_CONCOLIC_FIELDTRAVERSER_HPP
#define SHARE_VM_CONCOLIC_FIELDTRAVERSER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/defs.hpp"
#include "concolic/threadContext.hpp"
#include "memory/resourceArea.hpp"
#include "oops/instanceKlass.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/fieldDescriptor.hpp"
#include "runtime/handles.hpp"
#include "utilities/debug.hpp"

#include <stdio.h>
#include <vector>

class FieldTraverser : public FieldClosure {
private:
  int _depth, _target_depth;

protected:
  oop _obj;

  FieldTraverser(oop obj) : _obj(obj), _depth(1) {}
  void print_indent();

public:
  // recursive functions
  void do_recursive();
  void do_once();

private:
  virtual bool do_field_helper(fieldDescriptor *fd, oop obj) {}
  virtual bool do_element_helper(int index, arrayOop array_obj) {}
  virtual void after_field_helper(unsigned offset, oop temp_obj) {}
  virtual void after_element_helper(int index, oop temp_obj) {}
  virtual bool before_instance_helper() { return true; }
  virtual void after_instance_helper() {}
  virtual bool before_array_helper() { return true; }
  virtual void after_array_helper() {}

  void do_field(fieldDescriptor *fd);
  void do_array_element(int index);
  void do_recursive_helper();
  void do_array_elements(FieldTraverser *field_traverser);
};

class FieldSymbolizer : public FieldTraverser {
private:
  ThreadContext &_ctx;
  std::vector<SymRef *> _sym_refs;
  std::vector<std::string> _prefix_lst;

public:
  FieldSymbolizer(oop obj, ThreadContext &ctx)
      : FieldTraverser(obj), _ctx(ctx) {}
  FieldSymbolizer(oop obj, ThreadContext &ctx, std::string prefix)
      : FieldTraverser(obj), _ctx(ctx) {
    _prefix_lst.push_back(prefix);
  }
  bool print_field(fieldDescriptor *fd, oop obj);
  bool print_element(int index, arrayOop array_obj);

protected:
  bool do_field_helper(fieldDescriptor *fd, oop obj);
  bool do_element_helper(int index, arrayOop array_obj);
  void after_field_helper(unsigned offset, oop tem_obj);
  void after_element_helper(int index, oop tem_obj);
  virtual bool before_instance_helper();
  virtual void after_instance_helper();
  virtual bool before_array_helper();
  virtual void after_array_helper();

  std::string concatCurrentSymbolicName(fieldDescriptor* fd);
};

class SimpleFieldPrinter : public FieldTraverser {
public:
  SimpleFieldPrinter(oop obj) : FieldTraverser(obj) {}

protected:
  bool do_field_helper(fieldDescriptor *fd, oop obj);
  bool do_element_helper(int index, arrayOop array_obj);
};

class CompositeKeyGenerator : public FieldTraverser {
public:
  CompositeKeyGenerator(oop obj) : FieldTraverser(obj), _first(true) {}
  Expression* get_key_exp() {
    return _exp;
  }

protected:
  bool do_field_helper(fieldDescriptor *fd, oop obj);

private:
  bool _first;
  Expression *_exp;
  exp_list_t _param_list;
};

#endif // ENABLE_CONCOLIC
#endif // SHARE_VM_CONCOLIC_FIELDTRAVERSER_HPP