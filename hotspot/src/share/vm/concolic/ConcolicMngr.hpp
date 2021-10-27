#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

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
  static jlong startConcolic() {
    printf("Start concolic!\n");
    return 0;
  }

  static void symbolize(Handle handle) {
    handle()->print();
    handle()->set_handle(handle.raw_value());
  }
};

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP