#ifndef SHARE_VM_CONCOLIC_SHADOW_SHADOWSTACK_HPP
#define SHARE_VM_CONCOLIC_SHADOW_SHADOWSTACK_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/shadow/shadowFrame.hpp"

#include <vector>

class JavaThread;

class ShadowStack {
  typedef std::vector<ShadowFrame *> ShadowFrames;
  static const int DEFAULT_MAX_STACK_DEPTH = 1024;

private:
  ShadowFrames _s_frames;
  /**
   * Used to handle special parameter passing of Java reflection,
   */
  ShadowTable _reflection_stack;
  Expression *_reflection_ret_exp;

public:
  ShadowStack(JavaThread *jt);

  ~ShadowStack();

  ShadowTable *init_reflection_stack(int max_slot_size);

  inline ShadowTable &get_reflection_stack() { return _reflection_stack; }

  void set_reflection_ret_exp(Expression *exp) {
    assert(_reflection_ret_exp == NULL, "should be");
    _reflection_ret_exp = exp;
  }

  Expression *get_reflection_ret_exp() {
    Expression *ret = _reflection_ret_exp;
    _reflection_ret_exp = NULL;
    return ret;
  }

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

  void push(ZeroFrame *callee_frame, ZeroFrame *caller_frame, intptr_t *sp);

  void pop(ZeroFrame *zero_frame);

  void print_origin();

  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC__SHADOW_SHADOWSTACK_HPP