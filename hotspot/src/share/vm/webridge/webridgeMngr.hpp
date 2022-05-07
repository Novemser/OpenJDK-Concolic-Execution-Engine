//
// Created by gansen on 4/21/22.
//

#ifndef CLOSURE_RECEIVER_WEBRIDGEMNGR_HPP
#define CLOSURE_RECEIVER_WEBRIDGEMNGR_HPP

#include "concolic/threadContext.hpp"

class ThreadContext;

/**
 *
 */
class webridgeMngr {
#ifdef ENABLE_WEBRIDGE
//private:

public:
  static void analyse(ThreadContext *ctx, Klass* weBridgeSPEntryKlass);

#else
  public:
      static void analyse(ThreadContext *ctx, Klass* weBridgeSPEntryKlass) {}
#endif
};


#endif //CLOSURE_RECEIVER_WEBRIDGEMNGR_HPP
