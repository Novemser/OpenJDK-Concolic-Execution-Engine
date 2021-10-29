#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"

void SymbolicObject::symbolize(Handle handle) {
  tty->print("SymbolicObject::symbolize!\n");

  FieldSymbolizer::do_symbolize(handle());
}

void SymbolicObject::do_symbolize(oop obj, int depth) {
  if (obj->is_instance()) {
    FieldSymbolizer fieldSymbolizer(obj, depth, &FieldSymbolizer::print_field);
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

void FieldSymbolizer::do_symbolize(oop obj) {
  FieldSymbolizer fieldSymbolizer(obj, 1, &FieldSymbolizer::print_field);
  fieldSymbolizer.do_field_helper();
}

void FieldSymbolizer::do_field_helper() {
  if (_obj->is_instance()) {
    InstanceKlass *instanceKlass = (InstanceKlass *)_obj->klass();
    /**
     * Currently, we do not consider static fields
     */
    // instanceKlass->do_local_static_fields(&fieldSymbolizer);
    instanceKlass->do_nonstatic_fields(this);
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

  oop obj;
  if (fd->is_static()) { // check if static field

    // tty->print("static ");
    obj = ik->java_mirror(); // TODO: check whether we should use `java_mirror`
  } else {
    obj = _obj;
  }

  bool need_recursive = (this->*_field_handler)(fd, obj);
  if (need_recursive) {
    _depth += 1;
		oop temp_obj = this->_obj;
		this->_obj = obj->obj_field(fd->offset());
		this->do_field_helper();
		this->_obj = temp_obj;
    _depth -= 1;
  }
}

bool FieldSymbolizer::print_field(fieldDescriptor *fd, oop obj) {
	// output indents
  for (int i = 0; i < _depth; i++) {
    tty->print("    ");
  }

  // print `signature` and `name`
  tty->print("'%s' '%s' ", fd->signature()->as_C_string(),
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