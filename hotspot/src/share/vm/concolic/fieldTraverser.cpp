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
  } else if (_obj->is_array()) {
    arrayOop array_obj = (arrayOop)_obj;
    do_array_elements(this);
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

void FieldTraverser::do_array_elements(FieldTraverser* field_traverser) {
  assert(this->_obj->is_array(), "should be array");
  arrayOop array = (arrayOop)(this->_obj);
  int length = array->length();
  for (int index = 0; index < length; index++) {
    field_traverser->do_array_element(index);
  }
}

void FieldTraverser::do_array_element(int index) {
  // FIXME: for some object may refer to it self, resulting endless symbolizing.
  if (_depth > _target_depth)
    return;

  bool need_recursive;
  arrayOop array_obj = (arrayOop)_obj;
  need_recursive = this->do_array_element_helper(index, array_obj);

  if (need_recursive) {
    if (_obj->is_objArray()) {
      _depth += 1;
      oop temp_obj = this->_obj;
      this->_obj = ((objArrayOop)_obj)->obj_at(index);

      this->do_recursive_helper();

      this->_obj = temp_obj;
      _depth -= 1;
    } else {
      assert(false, "non-objarray won't be recursively done for now");
    }
  }
  
}

void FieldTraverser::print_indent() {
  for (int i = 0; i < _depth; i++) {
    tty->print("    ");
  }
}

/**************************************************
 * FieldSymbolizer
 */

bool FieldSymbolizer::do_field_helper(fieldDescriptor *fd, oop obj) {
  // print_indent();
  // tty->print("---- %d\n", fd->offset());

  // TODO: directly transfer SymInstance*
  SymInstance *sym_inst;
  SymArr *sym_arr;

  switch (fd->field_type()) {
  case T_OBJECT:
    return obj->obj_field(fd->offset()) != NULL;
  case T_ARRAY:
    sym_arr = this->_ctx.alloc_sym_array((arrayOop)(obj->obj_field(fd->offset())));
    sym_arr->set_length_exp(new SymbolExpression(sym_arr->get_sym_oid(), 0, 0));
    return false;
  default:
    sym_inst = this->_ctx.get_or_alloc_sym_inst(obj);
    sym_inst->init_sym_exp(fd->offset());
    return false;
  }
}

bool FieldSymbolizer::do_array_element_helper(int index, arrayOop array_obj) {
  ArrayKlass* array_klass = ArrayKlass::cast(array_obj->klass());

  SymInstance *sym_inst;
  BasicType element_type;
  element_type = array_klass->element_type();

  switch (element_type)
  {
  case T_OBJECT:
    return true;
  case T_ARRAY:
    assert(false, "element won't be array(it will be object)");
    return false;
  default:
    // the element_type is primitives
    sym_inst = this->_ctx.get_or_alloc_sym_inst(array_obj);
    sym_inst->init_sym_exp(index);
    return false;
  }
}

/**************************************************
 * SimpleFieldPrinter
 */
bool SimpleFieldPrinter::do_field_helper(fieldDescriptor *fd, oop obj) {
  this->print_indent();

  // print `signature` and `name`
  tty->print("'%d' '%s' '%s' ", fd->offset(), fd->signature()->as_C_string(),
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
  
bool SimpleFieldPrinter::do_array_element_helper(int index, arrayOop array_obj) {
  /**
   * TODO: complete this
   */
}

#endif