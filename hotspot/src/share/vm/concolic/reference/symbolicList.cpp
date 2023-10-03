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
  return set;
}

std::set<std::string> SymList::handle_method_names = init_handle_method_names();

std::set<std::string> SymList::init_handle_method_names() {
  std::set<std::string> set;
//  set.insert("add");
//  set.insert("get");
//  set.insert("remove");
//  set.insert("removeAll");
//  set.insert("addAll");
//  set.insert("contains");
//  set.insert("isEmpty");
//  set.insert("clear");
//  set.insert("size");
//  set.insert("toArray");
//  set.insert("empty");
  return set;
}

std::map<std::string, bool> SymList::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymList::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["<init>"] = false;// really? TODO: check whether this is needed
  map["sort"] = true;
  map["iterator"] = true;
  map["listIterator"] = true; // really???
  map["spliterator"] = true; // really???
  map["indexOf"] = true; // really???
  map["entries"] = true; // really???
  // not original List api, (belongs to hibernate.persistentBag)
  map["getOrphans"] = false;
  map["isSnapshotEmpty"] = false;
  map["getSnapshot"] = false;
  map["equalsSnapshot"] = false;
  map["needsRecreate"] = false;
  map["countOccurrences"] = false;
  map["isWrapper"] = false;
  map["forEach"] = false;
  return map;
}

void SymList::init_register_class(MethodSymbolizer *m_symbolizer) {
  for(std::set<std::string>::iterator it = target_class_names.begin(); it != target_class_names.end(); it++) {
    m_symbolizer->add_invoke_helper_methods(*it, invoke_method_helper);
    m_symbolizer->add_finish_helper_methods(*it, finish_method_helper);
  }
}

bool SymList::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = handle.general_check_param_symbolized();
    if (need_recording) {
//      handle.general_prepare_param();
    }
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
    } else {
      bool recording = handle.general_check_param_symbolized();
//      handle.get_callee_method()->print_name(tty);
//      tty->print_cr(" handled by SymList, need recording %c", recording ? 'Y' : 'N');
    }
  }

  return need_symbolize;
}


Expression *SymList::finish_method_helper(MethodSymbolizerHandle &handle) {
  if (!need_recording) {
    return NULL;
  }

  Expression *exp = NULL;
//  if (handle_method_names.find(handle.get_callee_name()) != handle_method_names.end()) {
//    exp = handle.general_prepare_result_helper();
//  }
  return exp;
}

#endif // ENABLE_CONCOLIC
