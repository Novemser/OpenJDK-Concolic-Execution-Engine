#ifdef ENABLE_CONCOLIC

#include "concolic/fieldSymbolizer.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void FieldSymbolizer::do_recursive_symbolize(oop obj) {
  FieldSymbolizer fieldSymbolizer(obj, &FieldSymbolizer::symbolize_field);
  fieldSymbolizer.do_field_helper();
}

void FieldSymbolizer::do_recursive_print(oop obj) {
  FieldSymbolizer fieldSymbolizer(obj, &FieldSymbolizer::print_field);
  fieldSymbolizer.do_field_helper();
}

void FieldSymbolizer::do_field_helper() {
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

void FieldSymbolizer::do_field(fieldDescriptor *fd) {
  // FIXME: for some object may refer to it self, resulting endless symbolizing.
  if (_depth > 3)
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
    need_recursive = (this->*_field_handler)(fd, obj);
  }

  if (need_recursive) {
    // before recursive
    _depth += 1;
    oop temp_obj = this->_obj;
    this->_obj = obj->obj_field(fd->offset());

    // recursive
    this->do_field_helper();

    // after recursive
    this->_obj = temp_obj;
    _depth -= 1;
  }
}

bool FieldSymbolizer::symbolize_field(fieldDescriptor *fd, oop obj) {
  tty->print("---- %d\n", fd->index());
	// SymbolicExpression* sym_exp = new SymbolicExpression(obj)
  switch (fd->field_type()) {
  case T_OBJECT:
    return obj->obj_field(fd->offset()) != NULL;
  case T_ARRAY:
    tty->print("unhandled\n");
    // TODO: return true;
    return false;
  default:
    return false;
  }
}

bool FieldSymbolizer::print_field(fieldDescriptor *fd, oop obj) {
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

void FieldSymbolizer::print_indent() {
  for (int i = 0; i < _depth; i++) {
    tty->print("    ");
  }
}

#endif