#ifdef ENABLE_CONCOLIC

#include "concolic/symbolicObject.hpp"
#include "utilities/ostream.hpp"

void FieldSymbolizer::do_field(fieldDescriptor* fd) {
    // FIXME: for some object may refer to it self, resulting endless symbolizing.
    if (_depth > 3) return;
    ResourceMark rm;
    InstanceKlass* ik = InstanceKlass::cast(_obj->klass());
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
    tty->print("'%s' '%s' ", fd->signature()->as_C_string(), fd->name()->as_C_string());
    oop sub_obj = NULL;
    switch (fd->field_type())
    {
    case T_BYTE:
        tty->print(" = %uc\n", obj->byte_field(fd->offset()));
        break;
    case T_CHAR:
        tty->print(" = %c\n", obj->char_field(fd->offset()));
        break;
    case T_DOUBLE:
        tty->print(" = %le\n", obj->double_field(fd->offset()));
        break;
    case T_FLOAT:
        tty->print(" = %e\n", obj->float_field(fd->offset()));
        break;
    case T_INT:
        tty->print(" = %d\n", obj->int_field(fd->offset()));
        break;
    case T_LONG:
        tty->print(" = %ld\n", obj->long_field(fd->offset()));
        break;
    case T_SHORT:
        tty->print(" = %hd\n", obj->short_field(fd->offset()));
        break;
    case T_BOOLEAN:
        tty->print(" = %d\n", obj->bool_field(fd->offset()));
        break;
    case T_VOID:
        tty->print("void\n");
    case T_OBJECT:
        tty->print("\n");
        sub_obj = obj->obj_field(fd->offset());
        if (sub_obj != NULL) {
            SymbolicObject::do_symbolize(obj->obj_field(fd->offset()), _depth+1);
        }
        break;
    case T_ARRAY:
        tty->print("unhandled\n");
        break;
    default:
        tty->print("illegal\n");
        assert(false, "illegal field type");
        break;
    }
}

void SymbolicObject::symbolize(Handle handle) {
    handle()->print();
    tty->print(" - ---- details\n");
    do_symbolize(handle(), 1);
}

void SymbolicObject::do_symbolize(oop obj, int depth) {
    if (obj->is_instance()) {
        FieldSymbolizer fieldSymbolizer(obj, depth);
        InstanceKlass* instanceKlass = (InstanceKlass*)obj->klass();
        instanceKlass->do_local_static_fields(&fieldSymbolizer);
        instanceKlass->do_nonstatic_fields(&fieldSymbolizer);
    } else {
        assert(false, "unhandled");
    }
}

#endif