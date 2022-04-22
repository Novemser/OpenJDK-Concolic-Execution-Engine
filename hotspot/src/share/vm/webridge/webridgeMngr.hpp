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
    static void analyse(ThreadContext *ctx);

#else
    public:
        static void analyse(ThreadContext *ctx) {}
#endif
};


#endif //CLOSURE_RECEIVER_WEBRIDGEMNGR_HPP
