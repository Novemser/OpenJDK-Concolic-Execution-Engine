#include "concolic/concolicMngr.hpp"
#include "concolic/threadContext.hpp"
#include "concolic/utils.hpp"
#include "webridge/webridgeMngr.hpp"
#include "utilities/ostream.hpp"
#include "utilities/vmError.hpp"

#ifdef ENABLE_CONCOLIC

volatile int ConcolicMngr::_num_threads_in_concolic = 0;
pthread_mutex_t ConcolicMngr::mutex = PTHREAD_MUTEX_INITIALIZER;
__thread ThreadContext *ConcolicMngr::ctx = NULL;

jlong ConcolicMngr::startConcolic(JavaThread *thread) {
  pthread_mutex_lock(&mutex);
  /**
   * Although ThreadContext is thread-local, there are some static fields of
   * other classes, which are still global.
   *
   * Therefore, we still assume that only one thread can execute in concolic
   * model
   */
  assert(_num_threads_in_concolic == 0, "should be");
  assert(thread != NULL, "not null java thread");

  ++_num_threads_in_concolic;

  ctx = new ThreadContext(thread);
  pthread_mutex_unlock(&mutex);
  tty->print_cr("Starting concolic execution");
  return 0;
}

jlong ConcolicMngr::endConcolic() {
  if (!ctx) {
    tty->print_cr("Warning: corrupted concolic execution thread state, is startConcolic() invoked properly?");
    return 1;
  }

  pthread_mutex_lock(&mutex);

  assert(_num_threads_in_concolic == 1, "should be");
  --_num_threads_in_concolic;

//  ctx->print();
  delete ctx;
  ctx = NULL;
  ThreadContext::memory_leak_check();
  pthread_mutex_unlock(&mutex);
  return 0;
}

void ConcolicMngr::printSymExp(Handle handle) {
  if (can_do_concolic()) {
    oop obj = handle();
    ctx->printSymExp(obj);
  }
}

void ConcolicMngr::symbolize(Handle handle) {
  if (can_do_concolic()) {
    tty->print("Symbolize!\n");
    ctx->symbolize(handle);
  } else {
    tty->print_cr("Warning: symbolize canned without proper ctx, try System.startConcolic() before symbolize");
  }
//  tty->print_cr("----- %s", ctx->get_code_pos_for_first("main").c_str());
}

void ConcolicMngr::symbolize(Handle handle, const char* prefix) {
  if (can_do_concolic()) {
    tty->print("Symbolize!\n");
    ctx->symbolize(handle, prefix);
  } else {
    tty->print_cr("Warning: symbolize canned without proper ctx, try System.startConcolic() before symbolize");
  }
//  tty->print_cr("----- %s", ctx->get_code_pos_for_first("main").c_str());
}

void ConcolicMngr::symbolizeMethod(Handle holder_name_handle,
                                   Handle callee_name_handle) {
  ResourceMark rm;
  const char *holder_name = OopUtils::java_string_to_c(holder_name_handle());
  const char *callee_name = OopUtils::java_string_to_c(callee_name_handle());

  ctx->symbolize_method(holder_name, callee_name);

  tty->print_cr("added symbolic method: %s.%s", holder_name, callee_name);
}

void ConcolicMngr::recordStmtObj(Handle stmt, Handle obj) {
  if (ctx == NULL) {
    // tty->print_cr(CL_RED"[warning] You should only call `recordStmtObj` after start concolic" CNONE);
    return;
  }
  ctx->record_stmt_obj(stmt(), obj());
}

void ConcolicMngr::recordPersistentObj(Handle obj) {
  if (ctx == NULL) {
    // tty->print_cr(CL_RED"[warning] You should only call `recordPersistentObj` after start concolic" CNONE);
    return;
  }
  ctx->record_persistent_obj(obj());
}

#else

jlong ConcolicMngr::startConcolic() {
  tty->print("Start concolic!\n");
  return 0;
}

jlong ConcolicMngr::endConcolic() {
  tty->print("End concolic!\n");
  return 0;
}

void ConcolicMngr::symbolize(Handle handle) {}

void ConcolicMngr::symbolizeMethod(Handle holder_name_handle,
                                   Handle callee_name_handle) {}
#endif