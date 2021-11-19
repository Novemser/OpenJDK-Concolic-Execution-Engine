#include "concolic/concolicMngr.hpp"
#include "concolic/threadContext.hpp"
#include "concolic/utils.hpp"
#include "utilities/ostream.hpp"
#include "utilities/vmError.hpp"

#ifdef ENABLE_CONCOLIC

bool ConcolicMngr::is_doing_concolic = false;
bool ConcolicMngr::is_symbolizing_method = false;
ThreadContext *ConcolicMngr::ctx = NULL;
MethodSymbolizer *ConcolicMngr::method_sym = NULL;


jlong ConcolicMngr::startConcolic(JavaThread *thread) {
  tty->print("Start concolic!\n");
  ConcolicMngr::is_doing_concolic = true;
  assert(thread != NULL, "not null java thread");
  ctx = new ThreadContext(thread);
  method_sym = new MethodSymbolizer;
  return 0;
}

jlong ConcolicMngr::endConcolic() {
  tty->print("End concolic!\n");
  ctx->print();
  delete method_sym;
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

void ConcolicMngr::symbolizeMethod(Handle holder_name_handle, Handle callee_name_handle) {
  ResourceMark rm;
  const char *holder_name = OopUtils::java_string_to_c(holder_name_handle());
  const char *callee_name = OopUtils::java_string_to_c(callee_name_handle());

  method_sym->add_symbolic_method(std::string(holder_name), std::string(callee_name));

  tty->print_cr("added symbolic method: %s.%s", holder_name, callee_name);
  method_sym->print();
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

void ConcolicMngr::symbolizeMethod(Handle holder_name_handle, Handle callee_name_handle) {}
#endif