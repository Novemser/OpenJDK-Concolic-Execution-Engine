#ifdef ENABLE_CONCOLIC

#include "symbolicStringUtils.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"
#include <concolic/exp/methodExpression.hpp>
#include <concolic/exp/stringExpression.hpp>

std::set<std::string> SymStrUtils::target_class_names = init_target_class_names();

method_set_t SymStrUtils::handle_method_names = init_handle_method_names();
std::map<std::string, bool> SymStrUtils::skip_method_names =
    init_skip_method_names();
bool SymStrUtils::need_recording = false;

method_set_t SymStrUtils::init_handle_method_names() {
  method_set_t m_set;
  m_set.insert("isNotBlank");
  m_set.insert("isEmpty");

  return m_set;
}

std::map<std::string, bool> SymStrUtils::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["<init>"] = true;
  return map;
}

std::set<std::string> SymStrUtils::init_target_class_names() {
  std::set<std::string> target_class_names;
  target_class_names.insert("org/apache/commons/lang/StringUtils");
  target_class_names.insert("org/springframework/util/StringUtils");
  return target_class_names;
}

void SymStrUtils::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}

bool SymStrUtils::invoke_method_helper(MethodSymbolizerHandle &handle) {
  guarantee(handle.get_callee_method()->is_static(), "should be");

  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = handle.general_check_param_symbolized();
    if (need_recording) {
      SymStrUtils::prepare_param(handle);
    }
  } else {
    std::map<std::string, bool>::iterator iter =
        skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
      if (!need_symbolize) {
        bool recording = handle.general_check_param_symbolized();
//        handle.get_callee_method()->print_name(tty);
//        tty->print_cr(" skipped by SymStrUtils, need recording %c",
//                      recording ? 'Y' : 'N');
      }
    } else {
      bool recording = handle.general_check_param_symbolized();
//      handle.get_callee_method()->print_name(tty);
//      tty->print_cr(" handled by SymStrUtils, need recording %c",
//                    recording ? 'Y' : 'N');
    }
  }

  return need_symbolize;
}

void SymStrUtils::prepare_param(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();

  int offset = handle.get_callee_local_begin_offset();

  if (!callee_method->is_static()) {
    SymStrUtils::prepare_param_helper(handle, T_OBJECT, offset);
    ++offset;
  }

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type()) {
    offset = SymStrUtils::prepare_param_helper(handle, ss.type(), offset);
    ss.next();
    ++offset;
  }
}

int SymStrUtils::prepare_param_helper(MethodSymbolizerHandle &handle,
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

Expression *SymStrUtils::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (need_recording) {
    const std::string &callee_name = handle.get_callee_name();
    BasicType type = handle.get_result_type();
    Expression *exp = NULL;
    oop obj = NULL;

    switch (type) {
      case T_VOID:
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



#endif
