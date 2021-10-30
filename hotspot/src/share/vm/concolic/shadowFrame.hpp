#ifndef SHARE_VM_CONCOLIC_SHADOWFRAME_HPP
#define SHARE_VM_CONCOLIC_SHADOWFRAME_HPP

#ifdef ENABLE_CONCOLIC

#include "runtime/frame.hpp"

class ShadowFrame {
private:
  frame _fr;
  int _fr_index;

public:
  ShadowFrame(frame &fr, int fr_index);
  ~ShadowFrame() {}

  void copy();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOWFRAME_HPP