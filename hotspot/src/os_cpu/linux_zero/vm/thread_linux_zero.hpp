/*
 * Copyright (c) 2000, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2007, 2008, 2009, 2010 Red Hat, Inc.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef OS_CPU_LINUX_ZERO_VM_THREAD_LINUX_ZERO_HPP
#define OS_CPU_LINUX_ZERO_VM_THREAD_LINUX_ZERO_HPP

 private:
  ZeroStack  _zero_stack;
  ZeroFrame* _top_zero_frame;
#ifdef ENABLE_WEBRIDGE
  bool _first_print_assertion;
#endif
  void pd_initialize() {
    _top_zero_frame = NULL;
  }

 public:
  ZeroStack *zero_stack() {
    return &_zero_stack;
  }

 public:
  ZeroFrame *top_zero_frame() {
    return _top_zero_frame;
  }

  void push_zero_frame(ZeroFrame *zframe);

  void pop_zero_frame();

 public:
  static ByteSize zero_stack_offset() {
    return byte_offset_of(JavaThread, _zero_stack);
  }
  static ByteSize top_zero_frame_offset() {
    return byte_offset_of(JavaThread, _top_zero_frame);
  }

 public:
  void inline __attribute__((optimize("O0"))) record_base_of_stack_pointer() {
#ifndef ENABLE_WEBRIDGE
    assert(top_zero_frame() == NULL, "junk on stack prior to Java call");
#else
    if (top_zero_frame() != NULL && !_first_print_assertion) {
      tty->print_cr("[Info] Unconfirmed assertion violated on record_base_of_stack_pointer");
      _first_print_assertion = true;
    }
#endif
  }
  void set_base_of_stack_pointer(intptr_t* base_sp) {
#ifndef ENABLE_WEBRIDGE
    assert(base_sp == NULL, "should be");
    assert(top_zero_frame() == NULL, "junk on stack after Java call");
#else
    if ((top_zero_frame() != NULL || base_sp != NULL) && !_first_print_assertion) {
      tty->print_cr("[Info] Unconfirmed assertion violated on set_base_of_stack_pointer");
      _first_print_assertion = true;
    }
#endif
  }

 public:
  void set_last_Java_frame() {
    set_last_Java_frame(top_zero_frame(), zero_stack()->sp());
  }
  void reset_last_Java_frame() {
    frame_anchor()->zap();
  }
  void set_last_Java_frame(ZeroFrame* fp, intptr_t* sp) {
    frame_anchor()->set(sp, NULL, fp);
  }

 public:
  ZeroFrame* last_Java_fp() {
    return frame_anchor()->last_Java_fp();
  }

 private:
  frame pd_last_frame() {
    assert(has_last_Java_frame(), "must have last_Java_sp() when suspended");
    return frame(last_Java_fp(), last_Java_sp());
  }

 public:
  static ByteSize last_Java_fp_offset() {
    return byte_offset_of(JavaThread, _anchor) +
      JavaFrameAnchor::last_Java_fp_offset();
  }

 public:
  // Check for pending suspend requests and pending asynchronous
  // exceptions.  There are separate accessors for these, but
  // _suspend_flags is volatile so using them would be unsafe.
  bool has_special_condition_for_native_trans() {
    return _suspend_flags != 0;
  }

 public:
  bool pd_get_top_frame_for_signal_handler(frame* fr_addr,
                                           void* ucontext,
                                           bool isInJava) {
    ShouldNotCallThis();
  }

  bool pd_get_top_frame_for_profiling(frame* fr_addr,
                                      void* ucontext,
                                      bool isInJava) {
    ShouldNotCallThis();
    return false; // silence compile warning
  }


  // These routines are only used on cpu architectures that
  // have separate register stacks (Itanium).
  static bool register_stack_overflow() { return false; }
  static void enable_register_stack_guard() {}
  static void disable_register_stack_guard() {}

#endif // OS_CPU_LINUX_ZERO_VM_THREAD_LINUX_ZERO_HPP
