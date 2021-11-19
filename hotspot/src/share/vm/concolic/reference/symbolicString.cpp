#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "concolic/utils.hpp"

const char *SymString::TYPE_NAME = "java/lang/String";
sym_rid_t SymString::sym_string_count = 0;

SymString::SymString(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _exp(NULL),
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

void SymString::print() {
  tty->print_cr("SymString: ");
  _ref_exp->print();
}

bool SymString::invoke_method(MethodSymbolizerHandle &handle) {
  if (handle.get_callee_name() == "startsWith") {
    int offset = handle.get_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();

    // this
    SymString::prepare_param(handle, T_OBJECT, locals, offset);
    ++offset;

    ResourceMark rm;
    SignatureStream ss(handle.get_callee_method()->signature());
    while (!ss.at_return_type()) {
      offset = SymString::prepare_param(handle, ss.type(), locals, offset);

      ss.next();
      ++offset;
    }
    assert(offset == handle.get_end_offset(), "equal");

    return true;
  }
  return false;
}

int SymString::prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                             intptr_t *locals, int offset) {
  Expression *exp;

  if (type == T_OBJECT) {
    oop obj = *(oop *)(locals - offset);
    exp = SymString::get_exp_of(obj);
  } else {
    offset += type2size[type] - 1;

    exp = ConcolicMngr::ctx->get_stack_slot(offset);
    if (!exp) {
      switch (type) {
      case T_INT:
        exp = new ConExpression(*(jint *)(locals - offset));
        break;
      default:
        ShouldNotReachHere();
        break;
      }
    }
  }

  handle.get_param_list().push_back(exp);
  return offset;
}

void SymString::finish_method(MethodSymbolizerHandle &handle) {
  BasicType type = handle.get_result_type();

  if (handle.get_callee_name() == "startsWith") {
    assert(type == T_BOOLEAN, "should be");
  }

  MethodSymbolizer::finish_method_helper(handle);
}

Expression *SymString::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    ResourceMark rm;
    exp = new StringExpression(OopUtils::java_string_to_c(obj));
  }
  return exp;
}

#endif