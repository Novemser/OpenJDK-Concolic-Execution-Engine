#include "concolic/concolicMngr.hpp"

bool ConcolicMngr::is_doing_concolic = false;
ThreadContext* ConcolicMngr::ctx = NULL;

#ifdef ENABLE_CONCOLIC

jlong ConcolicMngr::startConcolic() {
	printf("Start concolic!\n");
  ConcolicMngr::is_doing_concolic = true;
  ctx = new ThreadContext;
  return 0;
}

jlong ConcolicMngr::endConcolic() {
	printf("End concolic!\n");
	delete ctx;
	ConcolicMngr::is_doing_concolic = false;
	return 0;
}

void ConcolicMngr::symbolize(Handle handle) {
	handle()->print();
	ctx->symbolize(handle);
}

#else

jlong ConcolicMngr::startConcolic() {
	printf("Start concolic!\n");
  return 0;
}

jlong ConcolicMngr::endConcolic() {
	printf("End concolic!\n");
	return 0;
}

void ConcolicMngr::symbolize(Handle handle) {
}

#endif