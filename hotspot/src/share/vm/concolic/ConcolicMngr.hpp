#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "SymbolicStore.hpp"

#include "jvmtifiles/jvmti.h"
#include "runtime/handles.hpp"
#include "utilities/top.hpp"
#ifdef TARGET_OS_FAMILY_linux
# include "jvm_linux.h"
# include <setjmp.h>
#endif
#ifdef TARGET_OS_FAMILY_solaris
# include "jvm_solaris.h"
# include <setjmp.h>
#endif
#ifdef TARGET_OS_FAMILY_windows
# include "jvm_windows.h"
#endif
#ifdef TARGET_OS_FAMILY_aix
# include "jvm_aix.h"
# include <setjmp.h>
#endif
#ifdef TARGET_OS_FAMILY_bsd
# include "jvm_bsd.h"
# include <setjmp.h>
# ifdef __APPLE__
#  include <mach/mach_time.h>
# endif
#endif

class ConcolicMngr {
public:
  static bool is_doing_concolic;

  static jlong startConcolic() {
    printf("Start concolic!\n");
    ConcolicMngr::is_doing_concolic = true;
    return 0;
  }

  static jlong endConcolic() {
    printf("End concolic!\n");
    ConcolicMngr::is_doing_concolic = false;
    return 0;
  }

  static void symbolize(Handle handle) {
    handle()->print();
    handle()->set_handle(handle.raw_value());
  }
};

/**
 * Macro for CONCOLIC_BLOCK
 */ 
#ifdef ENABLE_CONCOLIC
  #define CONCOLIC_BLOCK_BEGIN \
    if (ConcolicMngr::is_doing_concolic) {
#else
  #define CONCOLIC_BLOCK_BEGIN \
    if (false) {
#endif

#define CONCOLIC_BLOCK_END }

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