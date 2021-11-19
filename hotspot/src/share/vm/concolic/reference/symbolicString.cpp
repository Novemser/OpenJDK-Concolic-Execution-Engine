#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "concolic/utils.hpp"

const char *SymString::TYPE_NAME = "java/lang/String";
sym_rid_t SymString::sym_string_count = 0;
SymString::Mset SymString::string_methods = init_string_methods();

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
  std::string callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (string_methods.find(callee_name) != string_methods.end()) {
    int offset = handle.get_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();

    // this
    SymString::prepare_param(handle, T_OBJECT, locals, offset, need_symbolize);
    ++offset;

    ResourceMark rm;
    SignatureStream ss(handle.get_callee_method()->signature());
    while (!ss.at_return_type()) {
      offset = SymString::prepare_param(handle, ss.type(), locals, offset, need_symbolize);

      ss.next();
      ++offset;
    }
    assert(offset == handle.get_end_offset(), "equal");

    return true;
  }
  return false;
}

int SymString::prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                             intptr_t *locals, int offset, bool &need_symbolize) {
  Expression *exp;

  if (type == T_OBJECT) {
    // only consider the situation that object is a string by now
    oop obj = *(oop *)(locals - offset);
    if (obj->is_symbolic()) {
      need_symbolize = true;
    }
    exp = SymString::get_exp_of(obj);
  } else {
    offset += type2size[type] - 1;

    exp = ConcolicMngr::ctx->get_stack_slot(offset);
    need_symbolize = exp ? true : need_symbolize;

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

SymString:: Mset SymString::init_string_methods() {
  SymString::Mset m_set;
  m_set.insert("charAt");
  m_set.insert("compareTo");
  m_set.insert("concat");
  m_set.insert("copyValueOf");
  m_set.insert("compareToIgnoreCase");
  m_set.insert("contentEquals");
  m_set.insert("contains");
  m_set.insert("endsWith");
  m_set.insert("equals");
  m_set.insert("equalsIgnoreCase");
  m_set.insert("getBytes");
  m_set.insert("getChars");
  m_set.insert("hashCode");
  m_set.insert("isEmpty");
  m_set.insert("indexOf");
  m_set.insert("intern");
  m_set.insert("lastIndexOf");
  m_set.insert("length");
  m_set.insert("matches");
  m_set.insert("regionMatches");
  m_set.insert("replace");
  m_set.insert("replaceAll");
  m_set.insert("replaceFirst");
  m_set.insert("split");
  m_set.insert("startsWith");
  m_set.insert("subSequence");
  m_set.insert("substring");
  m_set.insert("toCharArray");
  m_set.insert("toLowerCase");
  m_set.insert("toUpperCase");
  m_set.insert("trim");
  m_set.insert("valueOf");
  m_set.insert("<init>");
  return m_set;
}


#endif