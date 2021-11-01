#ifndef SHARE_VM_CONCOLIC_SHADOWFRAME_HPP
#define SHARE_VM_CONCOLIC_SHADOWFRAME_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/shadowTable.hpp"
#include "runtime/frame.hpp"

class ShadowFrame {

private:
  const ZeroFrame *_zero_frame;
  intptr_t *_sp;
  int _fr_index; // TODO: remove this useless field
  ShadowTable _opr_stack;
  ShadowTable _local_tbl;

public:
  ShadowFrame(frame &fr, int fr_index);
  ShadowFrame(const ZeroFrame *zero_frame, intptr_t *sp, int fr_index);
  ~ShadowFrame() {}

  void copy();

  void check(ZeroFrame *zero_frame);

  void print_origin();
  void print();

private:
  inline intptr_t *fp() { return (intptr_t *)_zero_frame; }
};

#endif

#endif // SHARE_VM_CONCOLIC_SHADOWFRAME_HPP