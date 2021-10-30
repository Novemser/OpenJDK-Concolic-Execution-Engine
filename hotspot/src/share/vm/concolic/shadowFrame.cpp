#ifdef ENABLE_CONCOLIC

#include "concolic/shadowFrame.hpp"
#include "utilities/ostream.hpp"

ShadowFrame::ShadowFrame(frame &fr, int fr_index)
    : _fr(fr), _fr_index(fr_index) {}

void ShadowFrame::copy() {
  static char sbuf[O_BUFLEN];
  char *buf = sbuf;
  int buflen = O_BUFLEN >> 1;
  char *fieldbuf = buf;
  char *valuebuf = buf + buflen;

  tty->print_cr("frame[%d]: %p, %lu", _fr_index, _fr.zeroframe(), sizeof(frame));
  // Print each word of the frame
  for (intptr_t *addr = _fr.sp(); addr <= _fr.fp(); addr++) {
    int offset = _fr.fp() - addr;

    // Fill in default values, then try and improve them
    snprintf(fieldbuf, buflen, "word[%d]", offset);
    snprintf(valuebuf, buflen, PTR_FORMAT, *addr);
    _fr.zeroframe()->identify_word(_fr_index, offset, fieldbuf, valuebuf,
                                   buflen);
    fieldbuf[buflen - 1] = '\0';
    valuebuf[buflen - 1] = '\0';

    // Print the result
    tty->print_cr(" " PTR_FORMAT ": %-21s = %s", p2i(addr), fieldbuf, valuebuf);
  }

  tty->cr();
}

#endif
