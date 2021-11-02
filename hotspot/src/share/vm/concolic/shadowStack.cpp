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

void ShadowStack::push(ZeroFrame *callee_zero_frame, ZeroFrame *caller_zero_frame,
                       intptr_t *sp) {
  // ConcolicMngr::ctx->print_stack_trace();
  // ConcolicMngr::ctx->get_shadow_stack().print();

  _s_frames.back()->check(caller_zero_frame);

  ShadowFrame &last_s_frame = get_frame(0);
  ShadowTable &last_opr_stack = last_s_frame.get_opr_stack();
  ShadowFrame *s_frame = new ShadowFrame(callee_zero_frame, sp, 8);
  s_frame->copy();
  /**
   * TODO: skip native here. Need to confirm correctness
   */
  interpreterState callee_istate = callee_zero_frame->as_interpreter_frame()->interpreter_state();
  Method *callee_method = callee_istate->method();
  if (!callee_method->is_native()) {
    interpreterState caller_istate = caller_zero_frame->as_interpreter_frame()->interpreter_state();
    /**
     * At this time, `istate->locals()` points to the start of locals,
     * while `caller_istate->stack()` points to end of locals
     */
    int begin_offset = caller_istate->stack_base() - callee_istate->locals() - 1;
    int end_offset = caller_istate->stack_base() - caller_istate->stack() - 1;
    s_frame->get_local_tbl().copy_entries(last_opr_stack, begin_offset, 0, end_offset - begin_offset);
  }
  _s_frames.push_back(s_frame);
}

void ShadowStack::pop(ZeroFrame *zero_frame) {
  /**
   * This is a workaround when we do not suppot shadow stack completely
   */
  assert(!_s_frames.empty(), "not empty");

  /**
   * do pop
   */
  ShadowFrame *s_frame = _s_frames.back();
  s_frame->check(zero_frame);
  _s_frames.pop_back();

  ShadowTable &opr_stack = s_frame->get_opr_stack();
  ShadowTable &next_opr_stack = get_last_frame().get_opr_stack();

  /**
   * return result from current opr_stack to locals
   */
  interpreterState callee_istate = zero_frame->as_interpreter_frame()->interpreter_state();
  Method* callee_method = callee_istate->method();
  // here `result` describes whatever returned
  int result_slots = type2size[callee_method->result_type()];
  assert(result_slots >= 0 && result_slots <= 2, "what?");
  
  intptr_t *callee_result = callee_istate->stack() + result_slots;
  int callee_opr_stack_offset = callee_istate->stack_base() - callee_result - 1;

  interpreterState caller_istate = zero_frame->next()->as_interpreter_frame()->interpreter_state();
  intptr_t *caller_result = caller_istate->stack_limit() + callee_method->max_locals();
  int caller_opr_stack_offset = caller_istate->stack_base() - caller_result - 1;
  /**
   * TODO: skip native here. Need to confirm correctness
   */
  if (!callee_method->is_native()) {
    next_opr_stack.copy_entries(opr_stack, callee_opr_stack_offset, caller_opr_stack_offset, 1);
    tty->print("\033[1;32m copy from %d to %d with size=%d\033[0m\n", callee_opr_stack_offset, caller_opr_stack_offset, result_slots);
  }

  delete s_frame;
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
