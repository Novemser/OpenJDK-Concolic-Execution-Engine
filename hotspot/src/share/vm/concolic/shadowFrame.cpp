#ifdef ENABLE_CONCOLIC

#include "concolic/shadowFrame.hpp"
#include "runtime/frame.inline.hpp"
#include "utilities/ostream.hpp"

ShadowFrame::ShadowFrame(frame &fr, int fr_index)
    : _fr(fr), _fr_index(fr_index) {}

void ShadowFrame::copy() {
  const ZeroFrame *zero_frame = _fr.zeroframe();
  if (zero_frame->is_interpreter_frame()) {
    const InterpreterFrame *interp_frame = zero_frame->as_interpreter_frame();
    const interpreterState istate = interp_frame->interpreter_state();
    Method *method = istate->method();
    /**
     * TODO: currently, the following methods return size in word,
     *    rather than size in object/primitive
     *    We need to fix this problem
     */
    int max_stack = method->max_stack();
    int max_locals = method->max_locals();

    _opr_stack.init(max_stack);
    _local_tbl.init(max_locals);

    // tty->print_cr("max_stack %d, max_locals %d", max_stack, max_locals);
  }
}

void ShadowFrame::print_origin() {
  static char sbuf[O_BUFLEN];
  char *buf = sbuf;
  int buflen = O_BUFLEN >> 1;
  char *fieldbuf = buf;
  char *valuebuf = buf + buflen;

  const ZeroFrame *zero_frame = _fr.zeroframe();

  tty->print_cr("frame[%d] of %p: ", _fr_index, zero_frame);
  // Print each word of the frame
  for (intptr_t *addr = _fr.sp(); addr <= _fr.fp(); addr++) {
    int offset = _fr.fp() - addr;

    // Fill in default values, then try and improve them
    snprintf(fieldbuf, buflen, "word[%d]", offset);
    snprintf(valuebuf, buflen, PTR_FORMAT, *addr);
    zero_frame->identify_word(_fr_index, offset, fieldbuf, valuebuf, buflen);
    fieldbuf[buflen - 1] = '\0';
    valuebuf[buflen - 1] = '\0';

    // Print the result
    tty->print_cr(" " PTR_FORMAT ": %-21s = %s", p2i(addr), fieldbuf, valuebuf);
  }

  if (zero_frame->is_interpreter_frame()) {
    const InterpreterFrame *interp_frame = zero_frame->as_interpreter_frame();
    const interpreterState istate = interp_frame->interpreter_state();

    tty->print_cr("stack %ld/%ld used", istate->stack() - istate->stack_limit(),
                  istate->stack() - istate->stack_limit());
  }

  tty->cr();
}

void ShadowFrame::print() {
  tty->print_cr("ShadowFrame[%d]: ", _fr_index);
  _opr_stack.print();
  _local_tbl.print();
}

#endif
