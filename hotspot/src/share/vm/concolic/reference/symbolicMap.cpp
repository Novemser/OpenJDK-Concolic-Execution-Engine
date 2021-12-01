#ifdef ENABLE_CONCOLIC

#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/reference/symbolicMap.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/utils.hpp"

bool SymMap::need_recording = false;

std::set<std::string> SymMap::target_class_names = init_target_class_names();

std::set<std::string> SymMap::init_target_class_names() {
  std::set<std::string> set;
  set.insert("java/util/HashMap");
  set.insert("org/hibernate/internal/util/collections/ConcurrentReferenceHashMap");
  return set;
}

std::set<std::string> SymMap::handle_method_names = init_handle_method_names();

std::set<std::string> SymMap::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("put");
  set.insert("putIfAbsent");
  set.insert("get");
  set.insert("containsKey");
  set.insert("isEmpty");
  set.insert("clear");
  return set;
}

std::set<std::string> SymMap::skip_method_names = init_skip_method_names();

std::set<std::string> SymMap::init_skip_method_names() {
  std::set<std::string> set;
  set.insert("<init>");
  set.insert("hash");
  return set;
}

bool SymMap::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    int offset = handle.get_callee_local_begin_offset();
    register intptr_t *locals = handle.get_locals_ptr();
    Method *callee_method = handle.get_callee_method();

    if (!callee_method->is_static()) {
      // handle this
      SymMap::prepare_param(handle, T_OBJECT, locals, offset,
                            need_recording);
      ++offset;
    }

    ResourceMark rm;
    SignatureStream ss(callee_method->signature());
    while (!ss.at_return_type()) {
      offset = SymMap::prepare_param(handle, ss.type(), locals, offset,
                                     need_recording);

      ss.next();
      ++offset;
    }
  } else if (skip_method_names.find(callee_name) == skip_method_names.end()) {
    handle.get_callee_method()->print_name(tty);
    tty->print_cr(" handled by SymMap");
  }

  return need_symbolize;
}

int SymMap::prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                          intptr_t *locals, int locals_offset,
                          bool &recording) {
  Expression *exp = NULL;

  if (type == T_OBJECT) {
    // only consider the situation that object is a string by now
    oop obj = *(oop *) (locals - locals_offset);
    if (obj != NULL) {
      if (obj->is_symbolic()) {
        recording = true;
        SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
        exp = sym_inst->get_ref_exp();
        if (exp == NULL) {
          exp = new InstanceSymbolExp(obj);
          sym_inst->set_ref_exp(exp);
        }
      }
      /**
       * TODO: handle expression for non-symbolic objects
       */
    }
  } else if (type == T_ARRAY) {
    arrayOop arr_obj = *(arrayOop *) (locals - locals_offset);
    if (arr_obj->is_symbolic()) {
      SymArr *sym_arr = ConcolicMngr::ctx->get_sym_array(arr_obj);
      exp = new ArraySymbolExp(arr_obj->get_sym_rid(), sym_arr->get_version(),
                               type);
      recording = true;
    } else {
      exp = new ArrayInitExpression(NULL_SYM_RID, arr_obj);
    }
  } else {
    tty->print_cr("unhandled map parameter types!");
  }

  handle.get_param_list().push_back(exp);
  return locals_offset;
}

Expression *SymMap::finish_method_helper(MethodSymbolizerHandle &handle) {
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
      default:
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