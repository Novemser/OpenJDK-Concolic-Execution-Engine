#ifdef ENABLE_CONCOLIC

#include "concolic/fieldTraverser.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void FieldTraverser::do_recursive() {
  this->_target_depth = 3; // TODO: unlimited
  this->do_recursive_helper();
}

void FieldTraverser::do_once() {
  this->_target_depth = 1;
  this->do_recursive_helper();
}

void FieldTraverser::do_recursive_helper() {
  if (_obj->is_instance()) {
    InstanceKlass *instanceKlass = (InstanceKlass *)_obj->klass();
    /**
     * Currently, we do not consider static fields
     */
    // instanceKlass->do_local_static_fields(this);
    instanceKlass->do_nonstatic_fields(this);
  } else {
    assert(false, "unhandled");
  }
}

void FieldTraverser::do_field(fieldDescriptor *fd) {
  // FIXME: for some object may refer to it self, resulting endless symbolizing.
  if (_depth > _target_depth)
    return;

  bool need_recursive;
  oop obj;
  {
    ResourceMark rm;
    InstanceKlass *ik = InstanceKlass::cast(this->_obj->klass());

    if (fd->is_static()) { // check if static field
      // tty->print("static ");
      obj =
          ik->java_mirror(); // TODO: check whether we should use `java_mirror`
    } else {
      obj = this->_obj;
    }
    need_recursive = this->do_field_helper(fd, obj);
  }

  if (need_recursive) {
    _depth += 1;
    oop temp_obj = this->_obj;
    this->_obj = obj->obj_field(fd->offset());

    this->do_recursive_helper();

    this->_obj = temp_obj;
    _depth -= 1;
  }
}

void FieldTraverser::print_indent() {
  for (int i = 0; i < _depth; i++) {
    tty->print("    ");
  }
}

bool FieldSymbolizer::do_field_helper(fieldDescriptor *fd, oop obj) {
  // print_indent();
  // tty->print("---- %d\n", fd->index());

  // TODO: directly transfer SymbolicObject*
  SymbolicObject *sym_obj;

  switch (fd->field_type()) {
  case T_OBJECT:
    return obj->obj_field(fd->offset()) != NULL;
  case T_ARRAY:
    assert(false, "unhandled!");
    // TODO: return true;
    return false;
  default:
    if (obj->is_symbolic()) {
      sym_obj = this->_ctx.get_sym_obj(obj->get_sym_oid());
    } else {
      sym_obj = this->_ctx.alloc_sym_obj(obj);
    }
    sym_obj->init_sym_exp(fd->index());
    return false;
  }
}

bool SimpleFieldPrinter::do_field_helper(fieldDescriptor *fd, oop obj) {
  this->print_indent();

  // print `signature` and `name`
  tty->print("'%d' '%s' '%s' ", fd->index(), fd->signature()->as_C_string(),
             fd->name()->as_C_string());

  switch (fd->field_type()) {
  case T_BYTE:
    tty->print(" = %uc\n", obj->byte_field(fd->offset()));
    return false;
  case T_CHAR:
    tty->print(" = %c\n", obj->char_field(fd->offset()));
    return false;
  case T_DOUBLE:
    tty->print(" = %le\n", obj->double_field(fd->offset()));
    return false;
  case T_FLOAT:
    tty->print(" = %e\n", obj->float_field(fd->offset()));
    return false;
  case T_INT:
    tty->print(" = %d\n", obj->int_field(fd->offset()));
    return false;
  case T_LONG:
    tty->print(" = %ld\n", obj->long_field(fd->offset()));
    return false;
  case T_SHORT:
    tty->print(" = %hd\n", obj->short_field(fd->offset()));
    return false;
  case T_BOOLEAN:
    tty->print(" = %d\n", obj->bool_field(fd->offset()));
    return false;
  case T_VOID:
    tty->print("void\n");
    return false;
  case T_OBJECT:
    tty->print("\n");
    return obj->obj_field(fd->offset()) != NULL;
  case T_ARRAY:
    tty->print("unhandled\n");
    // TODO: return true;
    return false;
  default:
    assert(false, "illegal field type");
    return false;
  }
}

#endif