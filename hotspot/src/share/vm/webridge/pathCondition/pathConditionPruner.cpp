//
// Created by novemser on 5/5/22.
//

#include "pathConditionPruner.hpp"

#ifdef ENABLE_WEBRIDGE

#include "runtime/frame.hpp"
#include "concolic/concolicMngr.hpp"

class_method_map_t PathConditionPruner::_classes_method_to_skip = init_skip_method_names();
std::set<std::string> PathConditionPruner::_classes_to_skip_name_prefix = init_classes_to_skip_name_prefix();
volatile int PathConditionPruner::_depth = 0;

void PathConditionPruner::method_enter(ZeroFrame *callee_frame) {
  if (!should_prune(callee_frame)) return;
  _depth++;
  guarantee(_depth > 0, "Inconsistent pc pruner depth");
  PathConditionPruner::disable_pc_recording();
}

void PathConditionPruner::method_exit(ZeroFrame *callee_frame) {
  if (!should_prune(callee_frame)) return;
  _depth--;
  guarantee(_depth >= 0, "Unexpected depth < 0");
  if (_depth == 0) {
    PathConditionPruner::enable_pc_recording();
  }
}

class_method_map_t PathConditionPruner::init_skip_method_names() {
  class_method_map_t classes;
  std::set<std::string> valBoxing;
  std::set<std::string> valBigDecimal;
  valBoxing.insert("valueOf");
  valBigDecimal.insert("<init>");
//  valBigDecimal.insert("setScale");
//  valBigDecimal.insert("valueOf");
//  valBigDecimal.insert("add");
//  valBigDecimal.insert("toString");
  classes["java/lang/Long"] = valBoxing;
  classes["java/lang/Byte"] = valBoxing;
  classes["java/lang/Short"] = valBoxing;
  classes["java/lang/Integer"] = valBoxing;
  classes["java/lang/Boolean"] = valBoxing;
  // empty methods indicate all methods should skip
//  classes["java/math/BigDecimal"] = valBigDecimal;
  classes["java/util/Locale"] = std::set<std::string>();
  classes["sun/util/locale/LocaleUtils"] = std::set<std::string>();
  classes["java/security/AccessController"] = std::set<std::string>();
  classes["java/lang/ClassLoader"] = std::set<std::string>();
  classes["sun/misc/Launcher"] = std::set<std::string>();
  classes["java/text/Format"] = std::set<std::string>();
  classes["java/text/NumberFormat"] = std::set<std::string>();
  classes["java/util/UUID"] = std::set<std::string>();
  classes["com/fasterxml/jackson/databind/ObjectWriter"] = std::set<std::string>();
  classes["java/text/SimpleDateFormat"] = std::set<std::string>();
  classes["ch/qos/logback/classic/Logger"] = std::set<std::string>();
  classes["org/springframework/security/web/firewall/StrictHttpFirewall"] = std::set<std::string>();
  classes["org/apache/catalina/connector/Request"] = std::set<std::string>();
  classes["com/salesmanager/shop/store/security/JWTTokenUtil"] = std::set<std::string>();
  return classes;
}

void PathConditionPruner::disable_pc_recording() {
  ConcolicMngr::ctx->set_pc_enabled(false);
}

void PathConditionPruner::enable_pc_recording() {
  ConcolicMngr::ctx->set_pc_enabled(true);
}

bool PathConditionPruner::should_prune(ZeroFrame *callee_frame) {
  if (!callee_frame || !callee_frame->is_interpreter_frame()) {
    return false;
  }

  const Method *callee_method =
      callee_frame->as_interpreter_frame()->interpreter_state()->method();
  if (!callee_method) {
    return false;
  }
  ResourceMark rm;
  const std::string mth_holder_name(
      callee_frame->as_interpreter_frame()->interpreter_state()
          ->method()->method_holder()->name()->as_C_string());

  if (_classes_method_to_skip.find(mth_holder_name) != _classes_method_to_skip.end()) {
    // empty methods indicate all methods should skip
    if (_classes_method_to_skip[mth_holder_name].empty()) {
      return true;
    }
    const std::string mth_name(callee_frame->as_interpreter_frame()->interpreter_state()
                                   ->method()->name()->as_C_string());
    if (_classes_method_to_skip[mth_holder_name].find(mth_name) !=
        _classes_method_to_skip[mth_holder_name].end()) {
      return true;
    }
  }

  std::set<std::string>::iterator _iter = _classes_to_skip_name_prefix.begin();
  for (; _iter != _classes_to_skip_name_prefix.end(); _iter++) {
    if (mth_holder_name.find(*_iter) != std::string::npos) return true;
  }

  return false;
}

std::set<std::string> PathConditionPruner::init_classes_to_skip_name_prefix() {
  std::set<std::string> s;
  s.insert("io/jsonwebtoken");
  s.insert("com/fasterxml");
  s.insert("edu/sjtu/ipads/wbridge/storedprocedure/invocation/SPInvokeManager");
  s.insert("java/io");
  s.insert("java/util/Calendar");
  s.insert("java/util/regex");
  s.insert("org/apache/commons");
  return s;
}

#endif