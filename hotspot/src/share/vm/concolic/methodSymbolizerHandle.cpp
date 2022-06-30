#ifdef ENABLE_CONCOLIC

#include <concolic/exp/methodExpression.hpp>
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


void MethodSymbolizerHandle::general_prepare_param(int max_param_num) {
  Method *callee_method = this->get_callee_method();
  int offset = this->get_callee_local_begin_offset();

  if (!callee_method->is_static()) {
    this->general_prepare_param_helper(T_OBJECT, offset, true);
    ++offset;
  }
  // handle this

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type() && offset < max_param_num) {
    offset = this->general_prepare_param_helper(ss.type(), offset, false);
    ss.next();
    ++offset;
  }
}

int __attribute__((optimize("O0"))) MethodSymbolizerHandle::general_prepare_param_helper(BasicType type,
                                                         int locals_offset, bool is_this) {
  Expression *exp = NULL;
  if (is_java_primitive(type)) {
    exp = this->get_primitive_exp(locals_offset, type);
    locals_offset += type2size[type] - 1;
  } else if (type == T_OBJECT) {
    oop obj = this->get_param<oop>(locals_offset);
    if (obj != NULL) {
      if (obj->is_symbolic()) {
        SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
        exp = sym_inst->get_or_create_ref_exp(obj);
      } else if (is_this) {
        SymInstance *sym_inst = ConcolicMngr::ctx->alloc_sym_inst(obj);
        exp = sym_inst->get_or_create_ref_exp(obj);
      } else {
        exp = SymInstance::get_exp_of(obj);
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

Expression __attribute__((optimize("O0")))  *MethodSymbolizerHandle::general_prepare_result_helper() {
  Expression *exp = NULL;
  BasicType type = this->get_result_type();
  oop obj = NULL;

  switch (type) {
    case T_VOID:
      exp = SymbolExpression::get(Sym_VOID);
      break;
    case T_OBJECT:
      obj = this->get_result<oop>(type);
      if (obj != NULL) {
        if (obj->is_symbolic()) {
          SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
          exp = sym_inst->get_or_create_ref_exp(obj);
        } else {
          exp = SymbolExpression::get(Sym_NULL);
        }
      } else {
        exp = SymbolExpression::get(Sym_NULL);
      }
      break;
    case T_BOOLEAN:
    case T_INT:
      exp = new MethodReturnSymbolExp(type);
      break;
    default:
      tty->print_cr("%c", type2char(type));
      this->get_callee_method()->print_name(tty);
      tty->cr();
      ShouldNotCallThis();
      break;
  }

  if (exp == NULL) {
    tty->print_cr("exp is NULL for ");
    obj->print();
  }

  ConcolicMngr::record_path_condition(new MethodExpression(
      this->get_callee_holder_name(), this->get_callee_name(),
      this->get_param_list(), exp));

  return exp;
}


#endif