//
// Created by novemser on 5/5/22.
//

#include "pathConditionPruner.hpp"

#ifdef ENABLE_WEBRIDGE

#include "runtime/frame.hpp"
#include "concolic/concolicMngr.hpp"

class_method_map_t PathConditionPruner::_classes_method_to_skip = init_skip_method_names();

void PathConditionPruner::method_enter(ZeroFrame *callee_frame) {
  if (should_prune(callee_frame)) {
    PathConditionPruner::disable_pc_recording();
  }
}

void PathConditionPruner::method_exit(ZeroFrame *callee_frame) {
  if (should_prune(callee_frame)) {
    PathConditionPruner::enable_pc_recording();
  }
}

class_method_map_t PathConditionPruner::init_skip_method_names() {
  class_method_map_t classes;
  std::set<std::string> valBoxing;
  std::set<std::string> valBigDecimal;
  valBoxing.insert("valueOf");
  valBigDecimal.insert("<init>");
//  valBigDecimal.insert("toString");
  classes["java/lang/Long"] = valBoxing;
  classes["java/lang/Byte"] = valBoxing;
  classes["java/lang/Short"] = valBoxing;
  classes["java/lang/Integer"] = valBoxing;
  classes["java/lang/Boolean"] = valBoxing;
  // empty methods indicate all methods should skip
  classes["java/math/BigDecimal"] = valBigDecimal;
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

  return false;
}

#endif