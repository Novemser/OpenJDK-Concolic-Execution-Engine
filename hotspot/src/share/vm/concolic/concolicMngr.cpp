#include "concolic/concolicMngr.hpp"
#include "utilities/ostream.hpp"

bool ConcolicMngr::is_doing_concolic = false;
ThreadContext *ConcolicMngr::ctx = NULL;

#ifdef ENABLE_CONCOLIC

jlong ConcolicMngr::startConcolic() {
  tty->print("Start concolic!\n");
  ConcolicMngr::is_doing_concolic = true;
  ctx = new ThreadContext;
  return 0;
}

jlong ConcolicMngr::endConcolic() {
  tty->print("End concolic!\n");
  delete ctx;
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