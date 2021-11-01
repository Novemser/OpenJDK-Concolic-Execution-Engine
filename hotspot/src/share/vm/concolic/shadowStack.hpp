#ifndef SHARE_VM_CONCOLIC_SHADOWSTACK_HPP
#define SHARE_VM_CONCOLIC_SHADOWSTACK_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/shadowFrame.hpp"

#include <vector>

class JavaThread;

class ShadowStack {
  typedef std::vector<ShadowFrame *> ShadowFrames;
  static const int DEFAULT_MAX_STACK_DEPTH = 32;

private:
  ShadowFrames _s_frames;

public:
  ShadowStack(JavaThread *jt);
  ~ShadowStack();

  inline ShadowFrame &get_last_frame() { return *_s_frames.back(); }

  void push(ZeroFrame *new_zero_frame, ZeroFrame *old_zero_frame, intptr_t *sp);
  void pop(ZeroFrame *zero_frame);

  void print_origin();
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOWSTACK_HPP