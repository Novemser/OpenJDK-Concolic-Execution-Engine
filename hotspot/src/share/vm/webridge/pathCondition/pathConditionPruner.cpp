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
  std::set<std::string> val;
  val.insert("valueOf");
  classes["java/lang/Long"] = val;
  classes["java/lang/Byte"] = val;
  classes["java/lang/Short"] = val;
  classes["java/lang/Integer"] = val;
  classes["java/lang/Boolean"] = val;
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