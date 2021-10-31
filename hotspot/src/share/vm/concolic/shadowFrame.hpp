#ifndef SHARE_VM_CONCOLIC_SHADOWFRAME_HPP
#define SHARE_VM_CONCOLIC_SHADOWFRAME_HPP

#ifdef ENABLE_CONCOLIC

#include "runtime/frame.hpp"
#include "concolic/shadowTable.hpp"

class ShadowFrame {
  
private:
  frame _fr;
  int _fr_index;
  ShadowTable _opr_stack;
  ShadowTable _local_tbl;

public:
  ShadowFrame(frame &fr, int fr_index);
  ~ShadowFrame() {}

  void copy();

  void print_origin();
  void print();
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOWFRAME_HPP