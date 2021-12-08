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
        exp = new ConExpression(this->get_param<jdouble>(offset));
        break;
      default:
        ShouldNotReachHere();
    }
  }
  return exp;
}

#endif