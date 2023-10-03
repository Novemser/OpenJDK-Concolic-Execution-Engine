#ifdef ENABLE_CONCOLIC

#include "concolic/fieldTraverser.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/reference/symbolicPrimitive.hpp"
#include "oops/klass.hpp"
#include "utilities/ostream.hpp"
#include "concolic/utils.hpp"
#include "concolic/exp/stringExpression.hpp"

#include <sstream>

void FieldTraverser::do_recursive() {
  this->_target_depth = 5; // TODO: unlimited
  this->do_recursive_helper();
}

void FieldTraverser::do_once() {
  this->_target_depth = 1;
  this->do_recursive_helper();
}

void FieldTraverser::do_recursive_helper() {
  if (_obj->is_instance()) {
    if (this->before_instance_helper()) {
      InstanceKlass *instanceKlass = (InstanceKlass *)_obj->klass();
      /**
       * Currently, we do not consider static fields
       */
      // instanceKlass->do_local_static_fields(this);
      instanceKlass->do_nonstatic_fields(this);
      this->after_instance_helper();
    }
  } else if (_obj->is_array()) {
    if (this->before_array_helper()) {
      do_array_elements(this);
      this->after_array_helper();
    }
  } else {
    assert(false, "unhandled");
  }
}

void FieldTraverser::do_field(fieldDescriptor *fd) {
  // FIXME: for some object may refer to it self, resulting endless symbolizing.
  if (_depth > _target_depth) {
    return;
  }

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
    unsigned offset = fd->offset();

    _depth += 1;
    oop parent_obj = this->_obj;
    this->_obj = obj->obj_field(offset);

    this->do_recursive_helper();
    this->after_field_helper(offset, parent_obj);

    this->_obj = parent_obj;
    _depth -= 1;
  }
}

void FieldTraverser::do_array_elements(FieldTraverser *field_traverser) {
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
  need_recursive = this->do_element_helper(index, array_obj);

  if (need_recursive) {
    if (_obj->is_objArray()) {
      _depth += 1;
      oop parent_obj = this->_obj;
      this->_obj = ((objArrayOop)_obj)->obj_at(index);

      this->do_recursive_helper();
      this->after_element_helper(index, parent_obj);

      this->_obj = parent_obj;
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

std::string FieldSymbolizer::concatCurrentSymbolicName(fieldDescriptor* fd) {
  ResourceMark rm;
  std::string name = _prefix_lst.back() + "_" + std::string(fd->name()->as_C_string());
  StringUtils::replaceAll(name, "/", "_");
  StringUtils::replaceAll(name, ":", "_");
  StringUtils::replaceAll(name, "$", "__");
  return name;
}

bool FieldSymbolizer::do_field_helper(fieldDescriptor *fd, oop obj) {
  // print_indent();
  // tty->print("---- %d\n", fd->offset());

  // TODO: directly transfer SymInstance*
  SymInstance *sym_inst;
  SymArr *sym_arr;

  BasicType type = fd->field_type();
  this->print_field(fd,obj);
  switch (type) {
  case T_OBJECT: {
    oop fld_oop = obj->obj_field(fd->offset());
    if (fld_oop == NULL) return false;
    if (fld_oop->klass()->name() == vmSymbols::java_lang_String()) {
      sym_inst = this->_ctx.get_sym_inst(obj);
      assert(sym_inst == (SymInstance *)this->_sym_refs.back(),
             "should be equal");
      SymString* sym_str_inst = reinterpret_cast<SymString *>(this->_ctx.get_or_alloc_sym_inst(fld_oop));
      sym_str_inst->set_ref_exp(new StringSymbolExp(obj->get_sym_rid(), concatCurrentSymbolicName(fd)));
      sym_inst->init_sym_exp(fd->offset(), sym_str_inst->get_ref_exp());
      return false;
    }
    return true;
  }
  case T_ARRAY: {
    arrayOop array_obj = (arrayOop)(obj->obj_field(fd->offset()));
    if (array_obj != NULL) {
      sym_arr = this->_ctx.alloc_sym_array(array_obj);
      sym_arr->set_length_exp(new ArrayLengthExp(sym_arr->get_sym_rid(), type));
    }
    return false;
  }
  default:
    sym_inst = this->_ctx.get_sym_inst(obj);
    assert(sym_inst == (SymInstance *)this->_sym_refs.back(),
           "should be equal");
    sym_inst->init_sym_exp(fd->offset(), type, concatCurrentSymbolicName(fd));
//      sym_inst->init_sym_exp(fd->offset(), type);
    return false;
  }
}

void FieldSymbolizer::after_field_helper(unsigned offset, oop parent_obj) {
  assert(parent_obj->is_symbolic(), "should be");
  assert(this->_obj->is_symbolic(), "should be");
}

bool FieldSymbolizer::before_instance_helper() {
  if (this->_obj->is_symbolic())
    return false;
  ResourceMark rm;
  SymInstance *sym_inst = this->_ctx.alloc_sym_inst(this->_obj);
  if (sym_inst && sym_inst->need_recursive()) {
    _sym_refs.push_back(sym_inst);

    char * clz_name = this->_obj->klass()->name()->as_C_string();
    tty->inc();
    if (!_prefix_lst.empty()) {
      _prefix_lst.push_back(_prefix_lst.back() + "_" + clz_name);
    } else {
      _prefix_lst.push_back(clz_name);
    }
    return true;
  } else if (this->_obj->klass()->name() == vmSymbols::java_lang_String()) {
    // rename plain String symbolic expressions
    SymString* sym_str_inst = reinterpret_cast<SymString *>(sym_inst);
    if (!_prefix_lst.empty()) {
      sym_str_inst->set_ref_exp(new StringSymbolExp(sym_inst->get_sym_rid(), _prefix_lst.back()));
    }
    return false;
  } else {
    return false;
  }
}

void FieldSymbolizer::after_instance_helper() {
    _sym_refs.pop_back();
    tty->dec();
    _prefix_lst.pop_back();
}

bool FieldSymbolizer::do_element_helper(int index, arrayOop array_obj) {
  ArrayKlass *array_klass = ArrayKlass::cast(array_obj->klass());

  SymArr *sym_arr;
  BasicType element_type;
  element_type = array_klass->element_type();

  switch (element_type) {
  case T_ARRAY:
    // element won't be array(it will be object)
    ShouldNotCallThis();
  case T_OBJECT:
    this->print_element(index,array_obj);
    return true;
  default:
    // the element_type is primitives
    sym_arr = this->_ctx.get_sym_array(array_obj);
    assert(sym_arr == (SymArr *)this->_sym_refs.back(), "should be equal");
    this->print_element(index,array_obj);
    return false;
  }
}

void FieldSymbolizer::after_element_helper(int index, oop parent_obj) {
  /**
   * TODO:
   */
}

bool FieldSymbolizer::before_array_helper() {
  if (this->_obj->is_symbolic())
    return false;

  arrayOop array_obj = (arrayOop)this->_obj;
  SymArr *sym_arr = this->_ctx.alloc_sym_array(array_obj);

  BasicType type = ArrayKlass::cast((array_obj)->klass())->element_type();
  sym_arr->set_length_exp(new ArrayLengthExp(sym_arr->get_sym_rid(), type));

  _sym_refs.push_back(sym_arr);
  ResourceMark rm;
  tty->indent().print_cr("symbolize array rid:%lu type:%c length:%d",array_obj->get_sym_rid(),type2char(type),array_obj->length());
  char * arr_clz_name = array_obj->klass()->name()->as_C_string();
  if (!_prefix_lst.empty()) {
    _prefix_lst.push_back(_prefix_lst.back() + arr_clz_name);
  } else {
    _prefix_lst.push_back(arr_clz_name);
  }
  tty->inc();
}

void FieldSymbolizer::after_array_helper() {
    _sym_refs.pop_back();
    tty->dec();
    _prefix_lst.pop_back();
}

/**************************************************
 * SimpleFieldPrinter
 */
bool SimpleFieldPrinter::do_field_helper(fieldDescriptor *fd, oop obj) {
  this->print_indent();
  ResourceMark rm;
  // print `signature` and `name`
  tty->print("'%d' '%s' '%s' '%d'", fd->offset(),
             fd->signature()->as_C_string(), fd->name()->as_C_string(),
             fd->index());

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

bool SimpleFieldPrinter::do_element_helper(int index, arrayOop array_obj) {
  /**
   * TODO: complete this
   */
}

bool CompositeKeyGenerator::do_field_helper(fieldDescriptor *fd, oop obj) {
  this->print_indent();
  Expression *exp;

  if (!_first) {
    _param_list.push_back(_exp);
    _param_list.push_back(new ConStringSymbolExp(std::string("-")));
    _exp = new OpStrExpression("concat", _param_list);
  }

  switch (fd->field_type()) {
    case T_OBJECT: {
      oop key_obj = obj->obj_field(fd->offset());
      Symbol *key_obj_klass_name = key_obj->klass()->name();
      if(key_obj_klass_name->equals("java/lang/Long")) {
        jlong val = OopUtils::java_long_to_c(key_obj);
        exp = OpStrExpression::to_string(SymPrimitive<jlong>::get_exp_of(key_obj));
      } else if (key_obj_klass_name->equals("java/lang/String")) {
        exp = SymString::get_exp_of(key_obj);
      } else {
        ShouldNotReachHere();
      }
      break;
    }
    default:
      ShouldNotReachHere();
  }

  if (!_first) {
    _param_list.push_back(_exp);
    _param_list.push_back(exp);
    _exp = new OpStrExpression("concat", _param_list);
  } else {
    _exp = exp;
    _first = false;
  }

  return false;
}

bool FieldSymbolizer::print_field(fieldDescriptor *fd, oop obj)  {
  ResourceMark rm;
  tty->indent().print("symbolize field %s rid:%lu index:%d type:%s offset:%d\n",
              fd->name()->as_C_string(), obj->get_sym_rid(), fd->index(),
                      fd->signature()->as_C_string(), fd->offset());
}

bool FieldSymbolizer::print_element(int index, arrayOop array_obj)  {
    ArrayKlass *ak = (ArrayKlass *)array_obj->klass();
    BasicType T = ak->element_type();
    tty->indent().print("symbolize element rid:%lu index:%d type:%c\n",
                         array_obj->get_sym_rid(), index,type2char(T));
}


#endif