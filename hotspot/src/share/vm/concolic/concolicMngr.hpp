#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "jni_zero.h"
#include "concolic/threadContext.hpp"
#include "runtime/handles.hpp"

class ConcolicMngr {
public:
  static bool is_doing_concolic;
  static ThreadContext* ctx;

  static jlong startConcolic(JavaThread* thread);
  static jlong endConcolic();
  static void symbolize(Handle handle);
};

/**
 * Macro for CONCOLIC_DEBUG_BLOCK
 */ 
#if defined(ENABLE_CONCOLIC) && defined(CONCOLIC_DEBUG)
  #define CONCOLIC_DEBUG_BLOCK_BEGIN \
    if (ConcolicMngr::is_doing_concolic) {
#else
  #define CONCOLIC_DEBUG_BLOCK_BEGIN \
    if (false) {
#endif

#define CONCOLIC_DEBUG_BLOCK_END }

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP