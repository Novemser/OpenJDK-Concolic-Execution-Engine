#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/jdbc/reference/symbolicHibernateKey.hpp"
#include "concolic/reference/symbolicMap.hpp"
#include "concolic/utils.hpp"

bool SymMap::need_recording = false;

std::set<std::string> SymMap::target_class_names = init_target_class_names();

std::set<std::string> SymMap::init_target_class_names() {
  std::set<std::string> set;
  set.insert("java/util/HashMap");
  set.insert("java/util/LinkedHashMap");
  set.insert("org/hibernate/internal/util/collections/ConcurrentReferenceHashMap");
  return set;
}

std::set<std::string> SymMap::handle_method_names = init_handle_method_names();

std::set<std::string> SymMap::init_handle_method_names() {
  std::set<std::string> set;
  set.insert("put");
  set.insert("putIfAbsent");
  set.insert("get");
  set.insert("remove");
  set.insert("containsKey");
  set.insert("isEmpty");
  set.insert("clear");
  set.insert("putAll");
  set.insert("size");
  /**
   * Please revise this operation when you have time!!!
   * Although it is symbolized, it might call some methods which should not be symbolized...
   */
  set.insert("merge");
  return set;
}

std::map<std::string, bool> SymMap::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymMap::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["<init>"] = true; //really??
  map["hash"] = true;
  map["keySet"] = true; // really?
  map["entrySet"] = true; // really?
  map["values"] = true; // really?
  map["forEach"] = false; // really?
  return map;
}

void SymMap::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}

bool SymMap::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = SymMap::check_param_symbolized(handle);
    if (need_recording) {
      SymMap::prepare_param(handle);
    }
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
      if (!need_symbolize) {
        bool recording = SymMap::check_param_symbolized(handle);
        if (recording) {
          handle.get_callee_method()->print_name(tty);
          tty->print_cr(" skipped by SymMap, need recording Y");
        }
      }
    } else {
      bool recording = SymMap::check_param_symbolized(handle);
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymMap, need recording %c", recording ? 'Y' : 'N');
    }
  }

  return need_symbolize;
}

void SymMap::prepare_param(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();
  guarantee(!callee_method->is_static(), "should be");

  int offset = handle.get_callee_local_begin_offset();
  // handle this
  SymMap::prepare_param_helper(handle, T_OBJECT, offset);
  ++offset;

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type()) {
    offset = SymMap::prepare_param_helper(handle, ss.type(), offset);
    ss.next();
    ++offset;
  }
}

int SymMap::prepare_param_helper(MethodSymbolizerHandle &handle, BasicType type,
                                 int locals_offset) {
  Expression *exp = NULL;

  if (type == T_OBJECT) {
    oop obj = handle.get_param<oop>(locals_offset);
    if (obj != NULL) {
      SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
      exp = sym_inst->get_or_create_ref_exp(obj);
    }
  } else {
    tty->print_cr("unhandled map parameter types!");
    ShouldNotCallThis();
  }

  handle.get_param_list().push_back(exp);
  return locals_offset;
}


bool SymMap::check_param_symbolized(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();
  guarantee(!callee_method->is_static(), "should be");
  int offset = handle.get_callee_local_begin_offset();
  bool recording = false;

  SymMap::check_param_symbolized_helper(handle, T_OBJECT, offset,
                                        recording);
  ++offset;

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  // Only when this or key object is symbolized, we symbolize Map
  if (!ss.at_return_type()) {
    SymMap::check_param_symbolized_helper(handle, ss.type(), offset,
                                          recording);
  }
  return recording;
}

int SymMap::check_param_symbolized_helper(MethodSymbolizerHandle &handle, BasicType type,
                                          int locals_offset,
                                          bool &recording) {
  oop obj = handle.get_param<oop>(locals_offset);
  if (obj != NULL && obj->is_symbolic()) {
    recording = true;
  }

  return locals_offset;
}


Expression *SymMap::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (!need_recording) {
    return NULL;
  }

  Expression *exp = NULL;
  if (handle_method_names.find(handle.get_callee_name()) != handle_method_names.end()) {
    exp = handle.general_prepare_result_helper();
  }
  return exp;
}

#endif // ENABLE_CONCOLIC
