#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"
#include <concolic/exp/methodExpression.hpp>
#include <concolic/exp/stringExpression.hpp>

const char *SymString::ARRAY_TYPE_NAME = "[Ljava/lang/String;";
const char *SymString::TYPE_NAME = "java/lang/String";
method_set_t SymString::handle_method_names = init_handle_method_names();
std::map<std::string, bool> SymString::skip_method_names =
    init_skip_method_names();
bool SymString::need_recording = false;

method_set_t SymString::init_handle_method_names() {
  method_set_t m_set;
  m_set.insert("charAt");
  m_set.insert("compareTo");
  m_set.insert("concat");
  m_set.insert("copyValueOf");
  m_set.insert("compareToIgnoreCase");
  m_set.insert("contains");
  m_set.insert("endsWith");
  m_set.insert("equals");
  m_set.insert("equalsIgnoreCase");
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
  m_set.insert("getBytes");
  m_set.insert("toString");
  return m_set;
}

std::map<std::string, bool> SymString::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["contentEquals"] = false;
  return map;
}

void SymString::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymString::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymString::TYPE_NAME, finish_method_helper);
}

SymString::SymString(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _exp(NULL), _ref_exp(NULL) {
  this->set_ref_exp(new StringSymbolExp(_sym_rid));
}

SymString::~SymString() {
  Expression::gc(_exp);
  Expression::gc(_ref_exp);
}

void SymString::print() {
  tty->print_cr("SymString: ");
  _ref_exp->print_cr();
}

bool SymString::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = handle.general_check_param_symbolized();
    if (need_recording) {
      SymString::prepare_param(handle);
    }
  } else {
    std::map<std::string, bool>::iterator iter =
        skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
      if (!need_symbolize) {
        bool recording = handle.general_check_param_symbolized();
        handle.get_callee_method()->print_name(tty);
        tty->print_cr(" skipped by SymString, need recording %c",
                      recording ? 'Y' : 'N');
      }
    } else {
      bool recording = handle.general_check_param_symbolized();
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymString, need recording %c",
                    recording ? 'Y' : 'N');
    }
  }

  return need_symbolize;
}

void SymString::prepare_param(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();

  int offset = handle.get_callee_local_begin_offset();

  if (!callee_method->is_static()) {
    SymString::prepare_param_helper(handle, T_OBJECT, offset);
    ++offset;
  }

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type()) {
    offset = SymString::prepare_param_helper(handle, ss.type(), offset);
    ss.next();
    ++offset;
  }
}

int SymString::prepare_param_helper(MethodSymbolizerHandle &handle,
                                    BasicType type, int locals_offset) {
  Expression *exp = NULL;

  if (is_java_primitive(type)) {
    exp = handle.get_primitive_exp(locals_offset, type);
    locals_offset += type2size[type] - 1;
  } else if (type == T_OBJECT) {
    oop obj = handle.get_param<oop>(locals_offset);
    guarantee(obj != NULL, "should be");
    exp = SymString::get_exp_of(obj);
  } else if (type == T_ARRAY) {
    tty->print_cr("record string method having a array param: ");
    handle.get_callee_method()->print_name(tty);
    tty->cr();

    arrayOop arr_obj = handle.get_param<arrayOop>(locals_offset);
    if (arr_obj->is_symbolic()) {
      SymArr *sym_arr = ConcolicMngr::ctx->get_sym_array(arr_obj);
      exp = new ArraySymbolExp(arr_obj->get_sym_rid(), sym_arr->get_version(),
                               type);
    } else {
      exp = new ArrayInitExpression(NULL_SYM_RID, arr_obj);
    }
  } else {
    tty->print_cr("unhandled String parameter types!");
    ShouldNotCallThis();
  }

  handle.get_param_list().push_back(exp);
  return locals_offset;
}

Expression *SymString::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (need_recording) {
    const std::string &callee_name = handle.get_callee_name();
    BasicType type = handle.get_result_type();
    Expression *exp = NULL;
    oop obj = NULL;

    switch (type) {
    case T_VOID:
      if (callee_name == "getChars") {
        ShouldNotReachHere();
      }
      exp = SymbolExpression::get(Sym_VOID);
      break;
    case T_OBJECT:
      obj = handle.get_result<oop>(type);
      if (obj != NULL) {
        guarantee(obj->klass()->name()->equals(SymString::TYPE_NAME),
                  "should be");
        ConcolicMngr::ctx->get_or_alloc_sym_inst(obj)->set_ref_exp(
            new OpStrExpression(callee_name, handle.get_param_list()));
      } else {
        exp = SymbolExpression::get(Sym_NULL);
      }
      break;
    case T_ARRAY:
      ShouldNotCallThis();
      break;
    default:
      exp = new OpStrExpression(callee_name, handle.get_param_list());
    }
    return exp;
  } else {
    return NULL;
  }
}

Expression *SymString::get_exp_of(oop obj) {
  if (obj->is_symbolic()) {
    return ConcolicMngr::ctx->get_sym_inst(obj)->get_ref_exp();
  } else {
    return new ConStringSymbolExp(obj);
  }
}

#endif