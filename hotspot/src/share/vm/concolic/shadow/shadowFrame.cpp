#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/shadow/shadowFrame.hpp"
#include "runtime/frame.inline.hpp"
#include "utilities/ostream.hpp"

ShadowFrame::ShadowFrame(frame &fr, int fr_index)
    : _zero_frame(fr.zeroframe()), _sp(fr.sp()), _fr_index(fr_index) {}

ShadowFrame::ShadowFrame(const ZeroFrame *zero_frame, intptr_t *sp,
                         int fr_index)
    : _zero_frame(zero_frame), _sp(sp), _fr_index(fr_index) {}

void ShadowFrame::copy() {
  // tty->print_cr("ShadowFrame Copy[%p]: ", _zero_frame);
  if (_zero_frame->is_interpreter_frame()) {
    const InterpreterFrame *interp_frame = _zero_frame->as_interpreter_frame();
    const interpreterState istate = interp_frame->interpreter_state();
    Method *method = istate->method();
    /**
     *  Be careful, the following methods return size in word,
     *    rather than size in parameters
     */
    int max_stack = method->max_stack();
    int max_locals = method->max_locals();

    _opr_stack.init(max_stack);
    _local_tbl.init(max_locals);

    // tty->print_cr("max_stack %d, max_locals %d", max_stack, max_locals);
  }
  /**
   * For EntryFrame, we currently only care about reflection, which is handled by ShadowStack
   */
}

void ShadowFrame::check(ZeroFrame *zero_frame) {
  guarantee(zero_frame == this->_zero_frame, "frame should be the same");
}

void ShadowFrame::print_origin() {
  static char sbuf[O_BUFLEN];
  char *buf = sbuf;
  int buflen = O_BUFLEN >> 1;
  char *fieldbuf = buf;
  char *valuebuf = buf + buflen;

  tty->print_cr("frame[%d] of %p: ", _fr_index, _zero_frame);
  // Print each word of the frame
  for (intptr_t *addr = _sp; addr <= fp(); addr++) {
    int offset = fp() - addr;

    // Fill in default values, then try and improve them
    snprintf(fieldbuf, buflen, "word[%d]", offset);
    snprintf(valuebuf, buflen, PTR_FORMAT, *addr);
    _zero_frame->identify_word(_fr_index, offset, fieldbuf, valuebuf, buflen);
    fieldbuf[buflen - 1] = '\0';
    valuebuf[buflen - 1] = '\0';

    // Print the result
    tty->print_cr(" " PTR_FORMAT ": %-21s = %s", p2i(addr), fieldbuf, valuebuf);
  }

  if (_zero_frame->is_interpreter_frame()) {
    const InterpreterFrame *interp_frame = _zero_frame->as_interpreter_frame();
    const interpreterState istate = interp_frame->interpreter_state();

    tty->print_cr("stack %ld/%ld used", istate->stack() - istate->stack_limit(),
                  istate->stack() - istate->stack_limit());
  }

  tty->cr();
}

void ShadowFrame::print() {
  tty->print_cr("ShadowFrame[%d]: ", _fr_index);
  tty->print_cr("- opr_stack:");
  _opr_stack.print();
  tty->print_cr("- local_tbl:");
  _local_tbl.print();
}

#endif
