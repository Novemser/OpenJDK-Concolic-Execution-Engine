#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "concolic/reference/symbolicSet.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

bool SymSet::need_recording = false;

std::set<std::string> SymSet::target_class_names = init_target_class_names();

std::set<std::string> SymSet::init_target_class_names() {
  std::set<std::string> set;
  set.insert("java/util/HashSet");
  set.insert("java/util/LinkedHashSet");
  set.insert("org/hibernate/collection/internal/PersistentSet");
  return set;
}

std::set<std::string> SymSet::handle_method_names = init_handle_method_names();

std::set<std::string> SymSet::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("add");
  set.insert("remove");
  set.insert("contains");
  set.insert("isEmpty");
  set.insert("clear");
  set.insert("size");
  return set;
}

std::map<std::string, bool> SymSet::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymSet::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["<init>"] = true; //really??
  map["iterator"] = true; // really???
  map["spliterator"] = true; // really???
  map["equalsSnapshot"] = true; // really???
  map["entries"] = true; // really???
  // not original List api, (belongs to hibernate.persistentBag)
  map["getSnapshot"] = false; // really???
  return map;
}

bool SymSet::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = SymSet::check_param_symbolized(handle);
    if (need_recording) {
      SymSet::prepare_param(handle);
    }
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
    } else {
      bool recording = SymSet::check_param_symbolized(handle);
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymSet, need recording %c", recording ? 'Y' : 'N');
    }
  }


//  else if (skip_method_names.find(callee_name) == skip_method_names.end()) {
//    handle.get_callee_method()->print_name(tty);
//    tty->print_cr(" handled by SymSet");
//  }
  return need_symbolize;
}

void SymSet::prepare_param(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();
  guarantee(!callee_method->is_static(), "should be");

  int offset = handle.get_callee_local_begin_offset();
  // handle this
  SymSet::prepare_param_helper(handle, T_OBJECT, offset);
  ++offset;

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type()) {
    offset = SymSet::prepare_param_helper(handle, ss.type(), offset);
    ss.next();
    ++offset;
  }
}

int SymSet::prepare_param_helper(MethodSymbolizerHandle &handle, BasicType type,
                                 int locals_offset) {
  Expression *exp = NULL;

  if (type == T_OBJECT) {
    oop obj = handle.get_param<oop>(locals_offset);
    if (obj != NULL) {
      SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
      exp = sym_inst->get_ref_exp();
      if (exp == NULL) {
        exp = new InstanceSymbolExp(obj);
        sym_inst->set_ref_exp(exp);
      }
    }
  } else {
    tty->print_cr("unhandled set parameter types!");
    ShouldNotCallThis();
  }

  handle.get_param_list().push_back(exp);
  return locals_offset;
}


bool SymSet::check_param_symbolized(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();
  guarantee(!callee_method->is_static(), "should be");
  int offset = handle.get_callee_local_begin_offset();
  bool recording = false;

  SymSet::check_param_symbolized_helper(handle, T_OBJECT, offset,
                                        recording);
  ++offset;

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  // Only when this or key object is symbolized, we symbolize Set
  if (!ss.at_return_type()) {
    SymSet::check_param_symbolized_helper(handle, ss.type(), offset,
                                          recording);
  }
  return recording;
}

int SymSet::check_param_symbolized_helper(MethodSymbolizerHandle &handle, BasicType type,
                                          int locals_offset,
                                          bool &recording) {
  oop obj = handle.get_param<oop>(locals_offset);
  if (obj != NULL && obj->is_symbolic()) {
    recording = true;
  }

  return locals_offset;
}

Expression *SymSet::finish_method_helper(MethodSymbolizerHandle &handle) {
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
        handle.get_callee_method()->print_name(tty);
        tty->cr();
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
