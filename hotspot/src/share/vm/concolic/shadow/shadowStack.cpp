#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/shadow/shadowStack.hpp"
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

void ShadowStack::push(ZeroFrame *callee_frame, ZeroFrame *caller_frame,
                       intptr_t *sp) {
  // ConcolicMngr::ctx->print_stack_trace();
  // ConcolicMngr::ctx->get_shadow_stack().print();

  _s_frames.back()->check(caller_frame);

  ShadowFrame &last_s_frame = get_frame(0);
  ShadowTable &last_opr_stack = last_s_frame.get_opr_stack();
  ShadowFrame *s_frame = new ShadowFrame(callee_frame, sp, 8);
  s_frame->copy();
  if (callee_frame->is_interpreter_frame()) {
    interpreterState callee_istate =
        callee_frame->as_interpreter_frame()->interpreter_state();
    Method *callee_method = callee_istate->method();
    if (caller_frame->is_interpreter_frame()) {
      /**
       * TODO: skip native here. Need to confirm correctness
       */
      if (!callee_method->is_native()) {
        interpreterState caller_istate =
            caller_frame->as_interpreter_frame()->interpreter_state();
        /**
         * At this time, `istate->locals()` points to the start of locals,
         * while `caller_istate->stack()` points to end of locals
         */
        int begin_offset =
            caller_istate->stack_base() - callee_istate->locals() - 1;
        int end_offset =
            caller_istate->stack_base() - caller_istate->stack() - 1;
        s_frame->get_local_tbl().copy_entries(last_opr_stack, begin_offset, 0,
                                              end_offset - begin_offset);
      } else {
        /**
         * For performance issue, we ignore native call,
         * as they so not support execute symbolically
         * TODO: heck the effect of this code path
         */
//        ShouldNotCallThis();
      }
    }
  } else if (callee_frame->is_entry_frame()) {
    EntryFrame *entry_frame = callee_frame->as_entry_frame();
    JavaCallWrapper *call_wrapper = *entry_frame->call_wrapper();
    Method *callee_method = call_wrapper->callee_method();

    ResourceMark rm;
    tty->print_cr("Entry frame before calling: %s", callee_method->name_and_sig_as_C_string());


    JavaThread *java_thread = ConcolicMngr::ctx->get_thread();
    ZeroStack *zero_stack = java_thread->zero_stack();

    int max_locals = (intptr_t *) callee_frame - zero_stack->sp() - 2;
    ShadowTable &reflection_stack = ConcolicMngr::ctx->get_shadow_stack().get_reflection_stack();
    if (reflection_stack.size() != 0) {
      assert(max_locals == reflection_stack.size(), "should be");
      s_frame->get_opr_stack().swap(reflection_stack);
    }
  }
  _s_frames.push_back(s_frame);
}

void ShadowStack::pop(ZeroFrame *callee_frame) {
  /**
   * This is a workaround when we do not suppot shadow stack completely
   */
  assert(!_s_frames.empty(), "not empty");

  /**
   * do pop
   */
  ShadowFrame *s_frame = _s_frames.back();
  s_frame->check(callee_frame);
  _s_frames.pop_back();

  ShadowTable &opr_stack = s_frame->get_opr_stack();
  ShadowTable &next_opr_stack = get_last_frame().get_opr_stack();

  /**
   * return result from current opr_stack to locals
   * TODO: we skip processing of `entry_frame` for now
   */
  if (callee_frame->is_interpreter_frame()) {
    interpreterState callee_istate =
        callee_frame->as_interpreter_frame()->interpreter_state();
    Method *callee_method = callee_istate->method();
    /**
     * TODO: skip native here. Need to confirm correctness
     */
    if (!callee_method->is_native()) {
      // here `result` describes whatever returned
      int result_slots = type2size[callee_method->result_type()];
      if (callee_istate->thread()->has_pending_exception()) {
        result_slots = 0;
      }
      assert(result_slots >= 0 && result_slots <= 2, "what?");

      intptr_t *callee_result = callee_istate->stack() + result_slots;
      /**
       * TODO: check callee_opr_stack_offset should be only 0 or 1?
       */
      int callee_opr_stack_offset =
          callee_istate->stack_base() - callee_result - 1;

      ZeroFrame *caller_frame = callee_frame->next();
      int caller_opr_stack_offset = 0;
      if (caller_frame->is_interpreter_frame()) {
        interpreterState caller_istate =
            caller_frame->as_interpreter_frame()->interpreter_state();
        intptr_t *caller_result = callee_istate->locals();
        /**
         * TODO: Reduandant calculation in MethodSymbolizerHandle
         */
        caller_opr_stack_offset =
            caller_istate->stack_base() - caller_result - 1;
      } else if (caller_frame->is_entry_frame()) {
        caller_opr_stack_offset = 0;
      }

      if (next_opr_stack.size() != 0) {
        next_opr_stack.copy_entries(opr_stack, callee_opr_stack_offset,
                                    caller_opr_stack_offset, result_slots);
      } else {
        assert(caller_frame->is_entry_frame(), "should be");
      }
      // tty->print_cr(CL_GREEN"copy from %d to %d with size=%d"CNONE,
      // callee_opr_stack_offset, caller_opr_stack_offset, result_slots);
    }
  }

  delete s_frame;
}

ShadowTable *ShadowStack::init_reflection_stack(int max_slot_size) {
  assert(_reflection_stack.size() == 0, "should be");
  _reflection_stack.init(max_slot_size);
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
