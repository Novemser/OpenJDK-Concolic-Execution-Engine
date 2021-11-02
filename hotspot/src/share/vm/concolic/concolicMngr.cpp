#include "concolic/concolicMngr.hpp"
#include "concolic/threadContext.hpp"
#include "utilities/ostream.hpp"
#include "utilities/vmError.hpp"

bool ConcolicMngr::is_doing_concolic = false;
ThreadContext *ConcolicMngr::ctx = NULL;

#ifdef ENABLE_CONCOLIC

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
  ConcolicMngr::is_doing_concolic = false;
  return 0;
}

void ConcolicMngr::symbolize(Handle handle) {
  tty->print("Symbolize!\n");
  ctx->symbolize(handle);
  // tty->print("\033[1;36m=================================================================\033[0m\n");
  // handle()->print();
  // tty->print("sym_oid: %ld, oop address: %p\n", handle()->get_sym_oid(), handle());
  // tty->print("\033[1;36m=================================================================\033[0m\n");
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