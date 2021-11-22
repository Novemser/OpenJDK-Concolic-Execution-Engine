#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "concolic/utils.hpp"

const char *SymString::TYPE_NAME = "java/lang/String";
method_set_t SymString::symbolized_methods = init_symbolized_methods();

method_set_t SymString::init_symbolized_methods() {
  method_set_t m_set;
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

SymString::SymString(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _exp(NULL),
      _ref_exp(new StringSymbolExp()) {
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
  _ref_exp->print_cr();
}

bool SymString::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  if (symbolized_methods.find(callee_name) != symbolized_methods.end()) {
    int offset = handle.get_callee_local_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();
    Method *callee_method = handle.get_callee_method();

    if (!callee_method->is_static()) {
      // handle this
      SymString::prepare_param(handle, T_OBJECT, locals, offset,
                               need_symbolize);
      ++offset;
    }

    ResourceMark rm;
    SignatureStream ss(callee_method->signature());
    while (!ss.at_return_type()) {
      offset = SymString::prepare_param(handle, ss.type(), locals, offset,
                                        need_symbolize);

      ss.next();
      ++offset;
    }
    // assert(offset == handle.get_end_offset(), "equal");
  }

  return need_symbolize;
}

int SymString::prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                             intptr_t *locals, int offset,
                             bool &need_symbolize) {
  Expression *exp;

  if (type == T_OBJECT) {
    // only consider the situation that object is a string by now
    oop obj = *(oop *)(locals - offset);
    if (obj->is_symbolic()) {
      need_symbolize = true;
    }
    exp = SymString::get_exp_of(obj);
  } else if (type == T_ARRAY) {
    arrayOop arr_obj = *(arrayOop *)(locals - offset);
    if (arr_obj->is_symbolic()) {
      SymArr *sym_arr = ConcolicMngr::ctx->get_sym_array(arr_obj);
      exp = new ArraySymbolExp(arr_obj->get_sym_rid(), sym_arr->get_version(),
                               type);
      need_symbolize = true;
    } else {
      exp = new ArrayInitExpression(NULL_SYM_RID, arr_obj);
    }
  } else {
    offset += type2size[type] - 1;

    exp = ConcolicMngr::ctx->get_stack_slot(offset);
    need_symbolize = exp ? true : need_symbolize;

    if (!exp) {
      switch (type) {
      case T_BOOLEAN:
        exp = new ConExpression(*(jboolean *)(locals - offset));
        break;
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

Expression *SymString::finish_method_helper(MethodSymbolizerHandle &handle) {
  return MethodSymbolizer::finish_method_helper(handle);
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