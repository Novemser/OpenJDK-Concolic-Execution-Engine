#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"

SymString::SymString(sym_rid_t sym_rid) : SymInstance(sym_rid) {}

SymString::~SymString() {
  if (_exp && _exp->dec_ref()) {
    delete _exp;
  }
}

Expression *SymString::get(int field_offset) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  return _exp;
}

void SymString::init_sym_exp(int field_offset) {
  init_sym_exp(field_offset,
               new SymbolExpression(this->get_sym_rid(), field_offset));
}

void SymString::init_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  exp->inc_ref();
  _exp = exp;
}

void SymString::set_sym_exp(int field_offset, Expression *exp) {
  assert(field_offset % 8 == 0,
         "we are turning to field_offset, this should be true");
  if (_exp && _exp->dec_ref()) {
    delete _exp;
  }

  _exp = exp;
  if (_exp) {
    _exp->inc_ref();
  }
}

void SymString::print() {}

bool SymString::invoke_method(MethodSymbolizer::Handle &handle) {
  if (handle.callee_name == "startsWith") {

    return true;
  }
  return false;
}

void SymString::finish_method(MethodSymbolizer::Handle &handle) {
  interpreterState caller_istate =
      handle.caller_frame->as_interpreter_frame()->interpreter_state();
  interpreterState callee_istate =
      handle.callee_frame->as_interpreter_frame()->interpreter_state();
  Method *callee = caller_istate->callee();
  int offset = caller_istate->stack_base() - callee_istate->locals() - 1;

  BasicType type = callee->result_type();
  Expression *exp;

  if (handle.callee_name == "startsWith") {
    if (type == T_OBJECT) {
      ShouldNotReachHere();
    } else if (type == T_ARRAY) {
      ShouldNotReachHere();
    } else {
      exp = new SymbolExpression();
      int delta = type2size[type] - 1;
      assert(delta >= 0, "should be");
      ConcolicMngr::ctx->set_stack_slot(offset + delta, exp);
    }
  }

  ConcolicMngr::record_path_condition(new MethodExpression(
      handle.callee_holder_name, handle.callee_name, handle.param_list, exp));
}

#endif