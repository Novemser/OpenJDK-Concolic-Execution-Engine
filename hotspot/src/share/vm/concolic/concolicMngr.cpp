#include "concolic/concolicMngr.hpp"
#include "concolic/threadContext.hpp"
#include "utilities/ostream.hpp"
#include "utilities/vmError.hpp"

#ifdef ENABLE_CONCOLIC

bool ConcolicMngr::is_doing_concolic = false;
ThreadContext *ConcolicMngr::ctx = NULL;


jlong ConcolicMngr::startConcolic(JavaThread *thread) {
  tty->print("Start concolic!\n");
  ConcolicMngr::is_doing_concolic = true;
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
  ConcolicMngr::is_doing_concolic = false;
  return 0;
}

void ConcolicMngr::symbolize(Handle handle) {
  tty->print("Symbolize!\n");
  ctx->symbolize(handle);
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

#endif