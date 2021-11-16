#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"

sym_rid_t SymString::sym_string_count = 0;

SymString::SymString(sym_rid_t sym_rid)
    : SymInstance(sym_rid),
      _ref_exp(new SymbolExpression("STR", sym_string_count++)) {
  _ref_exp->inc_ref();
}

SymString::~SymString() {
  if (_exp && _exp->dec_ref()) {
    delete _exp;
  }
  assert(_ref_exp, "not NULL");
  if (_ref_exp && _ref_exp->dec_ref()) {
    delete _ref_exp;
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
    interpreterState callee_istate =
        handle.callee_frame->as_interpreter_frame()->interpreter_state();
    interpreterState caller_istate =
        handle.caller_frame->as_interpreter_frame()->interpreter_state();
    Method *callee = callee_istate->method();

    int begin_offset =
        caller_istate->stack_base() - callee_istate->locals() - 1;
    int end_offset = caller_istate->stack_base() - caller_istate->stack() - 1;
    intptr_t *locals = ((intptr_t *)callee_istate->locals());

    // this
    SymString::prepare_param(handle, T_OBJECT, locals, begin_offset);
    ++begin_offset;

    ResourceMark rm;
    SignatureStream ss(callee->signature());
    while (!ss.at_return_type()) {
      begin_offset =
          SymString::prepare_param(handle, ss.type(), locals, begin_offset);

      ss.next();
      ++begin_offset;
    }
    assert(begin_offset == end_offset, "equal");

    return true;
  }
  return false;
}

int SymString::prepare_param(MethodSymbolizer::Handle &handle, BasicType type,
                             intptr_t *locals, int locals_offset) {
  Expression *exp;

  if (type == T_OBJECT) {
    oop obj = *(oop *)(locals - locals_offset);
    if (obj->is_symbolic()) {
      SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
      exp = sym_inst->get_ref_exp();
      assert(exp != NULL, "NOT NULL");
    } else {
      ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
      exp = new SymbolExpression(obj->get_sym_rid(),
                                 SymbolExpression::NULL_INDEX);
    }
  } else {
    locals_offset += type2size[type] - 1;

    exp = ConcolicMngr::ctx->get_stack_slot(locals_offset);
    if (!exp) {
      switch (type) {
      case T_INT:
        exp = new ConExpression(*(jint *)(locals - locals_offset));
        break;
      default:
        ShouldNotReachHere();
        break;
      }
    }
  }

  handle.param_list.push_back(exp);
  return locals_offset;
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