#ifndef SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP
#define SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP

#include "jvmtifiles/jvmti.h"
#include "runtime/atomic.hpp"
#include "runtime/extendedPC.hpp"
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

// #include <iostream>

class ConcolicMngr {
public:
  static jlong startConcolic(oop o) {
    o->print();
    printf("Start concolic!\n");
    return 0;
  }

};

#endif // SHARE_VM_CONCOLIC_CONCOLICMNGR_HPP