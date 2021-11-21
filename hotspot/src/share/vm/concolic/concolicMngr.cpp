#include "concolic/concolicMngr.hpp"
#include "concolic/threadContext.hpp"
#include "concolic/utils.hpp"
#include "utilities/ostream.hpp"
#include "utilities/vmError.hpp"

#ifdef ENABLE_CONCOLIC

ThreadContext *ConcolicMngr::ctx = NULL;

jlong ConcolicMngr::startConcolic(JavaThread *thread) {
  tty->print("Start concolic!\n");
  assert(thread != NULL, "not null java thread");
  ctx = new ThreadContext(thread);
  return 0;
}

jlong ConcolicMngr::endConcolic() {
  tty->print("End concolic!\n");
  ctx->print();
  delete ctx;
  tty->print_cr("Checking memory leaks for Expression, %lu remains...",
                Expression::total_count);
  ctx = NULL;
  return 0;
}

void ConcolicMngr::symbolize(Handle handle) {
  tty->print("Symbolize!\n");
  ctx->symbolize(handle);
}

void ConcolicMngr::symbolizeMethod(Handle holder_name_handle,
                                   Handle callee_name_handle) {
  ResourceMark rm;
  const char *holder_name = OopUtils::java_string_to_c(holder_name_handle());
  const char *callee_name = OopUtils::java_string_to_c(callee_name_handle());

  ctx->symbolize_method(holder_name, callee_name);

  tty->print_cr("added symbolic method: %s.%s", holder_name, callee_name);
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