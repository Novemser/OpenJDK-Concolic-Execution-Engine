/*
 * Copyright (c) 1997, 2010, Oracle and/or its affiliates. All rights reserved.
 * Copyright 2009, 2010 Red Hat, Inc.
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

#include "concolic/concolicMngr.hpp"
#include "precompiled.hpp"
#include "runtime/frame.inline.hpp"
#include "runtime/thread.inline.hpp"
#include "webridge/pathCondition/pathConditionPruner.hpp"

void JavaThread::cache_global_variables() {
  // nothing to do
}

void JavaThread::push_zero_frame(ZeroFrame *zframe) {
  *(ZeroFrame **)zframe = _top_zero_frame;
  _top_zero_frame = zframe;
#ifdef ENABLE_CONCOLIC
//  if (_top_zero_frame && _top_zero_frame->is_interpreter_frame()) {
//    ResourceMark rm;
//    std::string name = _top_zero_frame->as_interpreter_frame()->interpreter_state()->method()->name()->as_C_string();
//    if (name == "execute") {
//      tty->print_cr("intercepted executeQuery from push zero frame, ConcolicMngr::can_do_concolic():%d", ConcolicMngr::can_do_concolic());
//    }
//  }
  if (ConcolicMngr::can_do_concolic()) {
    ConcolicMngr::ctx->get_method_symbolizer().invoke_method(
        *(ZeroFrame **)zframe, _top_zero_frame);
  }
  if (ConcolicMngr::can_do_concolic()) {
    ConcolicMngr::ctx->get_shadow_stack().push(
        _top_zero_frame, *(ZeroFrame **)zframe, zero_stack()->sp());
  }
#ifdef ENABLE_WEBRIDGE
//  ConcolicMngr::method_enter_callback(_top_zero_frame, *(ZeroFrame **)zframe);
  if (ConcolicMngr::ctx != NULL) {
    PathConditionPruner::method_enter(_top_zero_frame);
  }
#endif
#endif
}

void JavaThread::pop_zero_frame() {
#ifdef ENABLE_CONCOLIC
  ZeroFrame *temp_frame = _top_zero_frame;
#endif

  zero_stack()->set_sp((intptr_t *)_top_zero_frame + 1);
  _top_zero_frame = *(ZeroFrame **)_top_zero_frame;
#ifdef ENABLE_CONCOLIC
  if (ConcolicMngr::can_do_concolic()) {
//    if (temp_frame && temp_frame->is_interpreter_frame()) {
//      tty->print_cr("Poping method: %s", temp_frame->as_interpreter_frame()->interpreter_state()->method()->name_and_sig_as_C_string());
//    }
    ConcolicMngr::ctx->get_shadow_stack().pop(temp_frame);
  }
  if (ConcolicMngr::has_symbolized_method()) {
    ConcolicMngr::ctx->get_method_symbolizer().finish_method(_top_zero_frame);
  } else if (ConcolicMngr::should_try_handle_method()) {
    // this is for those are not symbolized method
    ConcolicMngr::ctx->get_method_symbolizer().finish_handling_method(_top_zero_frame);
  }
#ifdef ENABLE_WEBRIDGE
//  ConcolicMngr::method_exit_callback(_top_zero_frame);
  if (ConcolicMngr::ctx != NULL) {
    PathConditionPruner::method_exit(temp_frame);
  }
#endif
#endif
}