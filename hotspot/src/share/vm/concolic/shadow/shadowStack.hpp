#ifndef SHARE_VM_CONCOLIC_SHADOW_SHADOWSTACK_HPP
#define SHARE_VM_CONCOLIC_SHADOW_SHADOWSTACK_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/shadow/shadowFrame.hpp"

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

  /**
   * get index-th ShadowFrame from top
   * no inline here, for debugging
   */
  ShadowFrame &get_frame(int index) {
    int size = _s_frames.size();
    assert(index < size, "invalid index");
    return *_s_frames.at(size - 1 - index); 
  }

  void push(ZeroFrame *new_zero_frame, ZeroFrame *old_zero_frame, intptr_t *sp);
  void pop(ZeroFrame *zero_frame);

  void print_origin();
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC__SHADOW_SHADOWSTACK_HPP