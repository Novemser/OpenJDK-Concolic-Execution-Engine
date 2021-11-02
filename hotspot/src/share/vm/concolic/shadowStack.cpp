#ifdef ENABLE_CONCOLIC

#include "concolic/shadowStack.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/thread.hpp"

#include <algorithm>

ShadowStack::ShadowStack(JavaThread *jt) {
  _s_frames.reserve(DEFAULT_MAX_STACK_DEPTH);

  if (jt->zero_stack()->sp() && jt->top_zero_frame()) {
    bool has_last_Java_frame = jt->has_last_Java_frame();
    if (!has_last_Java_frame)
      jt->set_last_Java_frame();

    // If the top frame is a Shark frame and the frame anchor isn't
    // set up then it's possible that the information in the frame
    // is garbage: it could be from a previous decache, or it could
    // simply have never been written.  So we print a warning...
    StackFrameStream sfs(jt);
    if (!has_last_Java_frame && !sfs.is_done()) {
      assert(!sfs.current()->zeroframe()->is_shark_frame(),
             "top frame might be junk");
    }

    for (int frame_index = 0; !sfs.is_done(); sfs.next(), frame_index++) {
      ShadowFrame *s_frame = new ShadowFrame(*sfs.current(), frame_index);
      s_frame->copy();
      _s_frames.push_back(s_frame);
    }
    // reverse the stack
    std::reverse(_s_frames.begin(), _s_frames.end());

    // Reset the frame anchor if necessary
    if (!has_last_Java_frame)
      jt->reset_last_Java_frame();
  }
}

ShadowStack::~ShadowStack() {
  for (ShadowFrames::iterator iter = _s_frames.begin(); iter != _s_frames.end();
       ++iter) {
    delete *iter;
  }
  _s_frames.clear();
}

void ShadowStack::push(ZeroFrame *new_zero_frame, ZeroFrame *old_zero_frame,
                       intptr_t *sp) {
  // ConcolicMngr::ctx->print_stack_trace();
  // ConcolicMngr::ctx->get_shadow_stack().print();

  _s_frames.back()->check(old_zero_frame);

  ShadowFrame &last_s_frame = get_frame(0);
  ShadowTable &last_opr_stack = last_s_frame.get_opr_stack();
  ShadowFrame *s_frame = new ShadowFrame(new_zero_frame, sp, 8);
  s_frame->copy();
  /**
   * TODO: skip native here. Need to confirm correctness
   */
  Method *new_method = new_zero_frame->as_interpreter_frame()->interpreter_state()->method();
  if (!new_method->is_native()) {
    int size = new_method->size_of_parameters();
    s_frame->copy_locals(last_opr_stack, size);
  }
  _s_frames.push_back(s_frame);
}

void ShadowStack::pop(ZeroFrame *zero_frame) {
  /**
   * This is a workaround when we do not suppot shadow stack completely
   */
  assert(!_s_frames.empty(), "not empty");

  ShadowFrame *s_frame = _s_frames.back();
  s_frame->check(zero_frame);
  delete s_frame;
  _s_frames.pop_back();
}

void ShadowStack::print_origin() {
  for (ShadowFrames::reverse_iterator iter = _s_frames.rbegin();
       iter != _s_frames.rend(); ++iter) {
    ShadowFrame *s_frame = *iter;
    s_frame->print_origin();
  }
}

void ShadowStack::print() {
  tty->print_cr("ShadowStack: ");
  for (ShadowFrames::reverse_iterator iter = _s_frames.rbegin();
       iter != _s_frames.rend(); ++iter) {
    ShadowFrame *s_frame = *iter;
    s_frame->print();
    tty->print("\n");
  }
}

#endif
