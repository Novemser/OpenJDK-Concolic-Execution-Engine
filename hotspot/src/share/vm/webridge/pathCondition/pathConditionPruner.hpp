//
// Created by novemser on 5/5/22.
//

#ifndef OPENJDK8_PATHCONDITIONPRUNER_HPP
#define OPENJDK8_PATHCONDITIONPRUNER_HPP
#ifdef ENABLE_WEBRIDGE
#include <set>
#include <string>
class ZeroFrame;
#include "webridge/defs.hpp"

class PathConditionPruner {
public:
  static void method_enter(ZeroFrame* callee_frame);
  static void method_exit(ZeroFrame* callee_frame);

private:
  static class_method_map_t init_skip_method_names();
  static class_method_map_t _classes_method_to_skip;

  static bool should_prune(ZeroFrame* callee_frame);

  static void disable_pc_recording();
  static void enable_pc_recording();
};

#endif
#endif //OPENJDK8_PATHCONDITIONPRUNER_HPP
