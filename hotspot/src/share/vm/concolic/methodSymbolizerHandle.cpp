#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/methodSymbolizerHandle.hpp"

Expression *MethodSymbolizerHandle::get_primitive_exp(int offset, BasicType type) {
  Expression *exp;
  offset += type2size[type] - 1;
  exp = ConcolicMngr::ctx->get_stack_slot(this->get_caller_stack_begin_offset() + offset);
  if (!exp) {
    switch (type) {
      case T_BYTE:
        exp = new ConExpression(this->get_param<jbyte>(offset));
        break;
      case T_BOOLEAN:
        exp = new ConExpression(this->get_param<jboolean>(offset));
        break;
      case T_INT:
        exp = new ConExpression(this->get_param<jint>(offset));
        break;
      case T_SHORT:
        exp = new ConExpression(this->get_param<jshort>(offset));
        break;
      case T_LONG:
        exp = new ConExpression(this->get_param<jlong>(offset));
        break;
      case T_FLOAT:
        exp = new ConExpression(this->get_param<jfloat>(offset));
        break;
      case T_DOUBLE:
        exp = new ConExpression(this->get_param<jdouble>(offset));
        break;
      case T_CHAR:
        exp = new ConExpression(this->get_param<jchar>(offset));
        break;
      default:
        ShouldNotReachHere();
    }
  }
  return exp;
}


void MethodSymbolizerHandle::general_prepare_param() {
  Method *callee_method = this->get_callee_method();
  int offset = this->get_callee_local_begin_offset();

  if (!callee_method->is_static()) {
    this->general_prepare_param_helper(T_OBJECT, offset);
    ++offset;
  }
  // handle this

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type()) {
    offset = this->general_prepare_param_helper(ss.type(), offset);
    ss.next();
    ++offset;
  }
}

int MethodSymbolizerHandle::general_prepare_param_helper(BasicType type,
                                                         int locals_offset) {
  Expression *exp = NULL;
  if (is_java_primitive(type)) {
    exp = this->get_primitive_exp(locals_offset, type);
  } else if (type == T_OBJECT) {
    oop obj = this->get_param<oop>(locals_offset);
    if (obj != NULL) {
      SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
      exp = sym_inst->get_ref_exp();
      if (exp == NULL) {
        exp = new InstanceSymbolExp(obj);
        sym_inst->set_ref_exp(exp);
      }
    }
  } else {
    tty->print_cr("un_handled type %c", type2char(type));
    ShouldNotCallThis();
  }

  this->get_param_list().push_back(exp);
  return locals_offset;
}

bool MethodSymbolizerHandle::general_check_param_symbolized() {
  Method *callee_method = this->get_callee_method();
  int offset = this->get_callee_local_begin_offset();
  bool recording = false;

  if (!callee_method->is_static()) {
    recording |= this->general_check_param_symbolized_helper(T_OBJECT, offset);
    ++offset;
  }
  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  // Only when this or key object is symbolized, we symbolize Map
  while (!ss.at_return_type()) {
    recording |= this->general_check_param_symbolized_helper(ss.type(), offset);
    ss.next();
    ++offset;
  }
  return recording;

}


bool MethodSymbolizerHandle::general_check_param_symbolized_helper(BasicType type, int &locals_offset) {
  bool recording = false;
  if (is_java_primitive(type)) {
    locals_offset += type2size[type] - 1;
    Expression *exp = ConcolicMngr::ctx->get_stack_slot(this->get_caller_stack_begin_offset() + locals_offset);
    if (exp != NULL) {
      recording = true;
    }
  } else if (type == T_OBJECT || type == T_ARRAY) {
    oop obj = this->get_param<oop>(locals_offset);
    if (obj != NULL && obj->is_symbolic()) {
      recording = true;
    }
  } else {
    ShouldNotCallThis();
  }

  return recording;
}


#endif