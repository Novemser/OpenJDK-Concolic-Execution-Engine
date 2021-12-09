#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "concolic/reference/symbolicList.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

bool SymList::need_recording = false;

std::set<std::string> SymList::target_class_names = init_target_class_names();

std::set<std::string> SymList::init_target_class_names() {
  std::set<std::string> set;
  set.insert("java/util/List");
  set.insert("java/util/ArrayList");
  set.insert("org/hibernate/collection/internal/PersistentBag");
  return set;
}

std::set<std::string> SymList::handle_method_names = init_handle_method_names();

std::set<std::string> SymList::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("add");
  set.insert("get");
  set.insert("remove");
  set.insert("contains");
  set.insert("isEmpty");
  set.insert("clear");
  return set;
}

std::map<std::string, bool> SymList::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymList::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["<init>"] = true;// really?
  map["iterator"] = true;
  map["sort"] = true;
  map["listIterator"] = true; // really???
  map["addAll"] = true; // really???
  map["toArray"] = true; // really???
  map["indexOf"] = true; // really???
  map["spliterator"] = true; // really???
  map["equalsSnapshot"] = true; // really???
  map["getSnapshot"] = true; // really???
  map["entries"] = true; // really???
  map["getOrphans"] = true; // really???
  map["size"] = false; // really???
  return map;
}

bool SymList::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = handle.general_check_param_symbolized();
    if (need_recording) {
      handle.general_prepare_param();
    }
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
      if (!need_symbolize) {
        bool recording = handle.general_check_param_symbolized();
        handle.get_callee_method()->print_name(tty);
        tty->print_cr(" skipped by SymList, need recording %c", recording ? 'Y' : 'N');
      }
    } else {
      bool recording = handle.general_check_param_symbolized();
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymList, need recording %c", recording ? 'Y' : 'N');
    }
  }

  return need_symbolize;
}


Expression *SymList::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (!need_recording) {
    return NULL;
  }

  Expression *exp = NULL;
  if (handle_method_names.find(handle.get_callee_name()) != handle_method_names.end()) {

    BasicType type = handle.get_result_type();
    oop obj = NULL;

    switch (type) {
      case T_VOID:
        exp = SymbolExpression::get(Sym_VOID);
        break;
      case T_OBJECT:
        obj = handle.get_result<oop>(type);
        if (obj != NULL) {
          if (obj->is_symbolic()) {
            exp = ConcolicMngr::ctx->get_sym_inst(obj)->get_ref_exp();
            SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
            if (exp == NULL) {
              exp = new InstanceSymbolExp(obj);
              sym_inst->set_ref_exp(exp);
            }
          }
        } else {
          exp = SymbolExpression::get(Sym_NULL);
        }
        break;
      case T_BOOLEAN:
      case T_INT:
        exp = new MethodReturnSymbolExp();
        break;
      default:
        tty->print_cr("%c", type2char(type));
        ShouldNotCallThis();
        break;
    }

    ConcolicMngr::record_path_condition(new MethodExpression(
        handle.get_callee_holder_name(), handle.get_callee_name(),
        handle.get_param_list(), exp));
  }
  return exp;
}

#endif // ENABLE_CONCOLIC
