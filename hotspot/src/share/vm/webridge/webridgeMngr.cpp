//
// Created by gansen on 4/21/22.
//

#include "webridgeMngr.hpp"
#ifdef ENABLE_WEBRIDGE
void webridgeMngr::analyse(ThreadContext *ctx) {
    // 1. Construct a "SQL graph" from concolic execution manager.
//    auto jdbc_mgr = ctx->get_jdbc_mngr();

    // 2. Translate the graph into Stored procedure.
}
#endif