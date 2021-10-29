#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void SymbolicObject::symbolize(Handle handle) {
  tty->print("SymbolicObject::symbolize!\n");

  do_symbolize(handle(), 1);
}

void SymbolicObject::do_symbolize(oop obj, int depth) {
  if (obj->is_instance()) {
    FieldSymbolizer fieldSymbolizer(obj, depth, FieldSymbolizer::print_field);
    InstanceKlass *instanceKlass = (InstanceKlass *)obj->klass();

    /**
     * Currently, we do not consider static fields
     */
    // instanceKlass->do_local_static_fields(&fieldSymbolizer);
    instanceKlass->do_nonstatic_fields(&fieldSymbolizer);
  } else {
    assert(false, "unhandled");
  }
}

void FieldSymbolizer::do_field(fieldDescriptor *fd) {
  // FIXME: for some object may refer to it self, resulting endless symbolizing.
  if (_depth > 3)
    return;
  ResourceMark rm;
  InstanceKlass *ik = InstanceKlass::cast(_obj->klass());
  // output indents
  for (int i = 0; i < _depth; i++) {
    tty->print("    ");
  }
  // check if static field
  oop obj;
  if (fd->is_static()) {
    tty->print("static ");
    obj = ik->java_mirror(); // TODO: check whether we should use `java_mirror`
  } else {
    obj = _obj;
  }
  // print `signature` and `name`
  tty->print("'%s' '%s' ", fd->signature()->as_C_string(),
             fd->name()->as_C_string());

  bool need_recursive = _field_handler(fd, obj);
  if (need_recursive) {
    SymbolicObject::do_symbolize(obj->obj_field(fd->offset()), _depth + 1);
  }
}

bool FieldSymbolizer::print_field(fieldDescriptor *fd, oop obj) {
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
  case T_OBJECT: {
    tty->print("\n");
    oop sub_obj = obj->obj_field(fd->offset());
    return sub_obj != NULL;
  }
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