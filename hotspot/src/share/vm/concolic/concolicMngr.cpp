#include "concolic/concolicMngr.hpp"
#include "concolic/threadContext.hpp"
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

typeArrayOop ConcolicMngr::charArrayObjFromJavaString(oop str_obj) {
  Klass *klass = str_obj->klass();
  assert(klass->name()->equals("java/lang/String"), "should be String");
  InstanceKlass *ik = (InstanceKlass*)klass;

  const int value_field_index = 0;
  assert(ik->field_name(value_field_index)->equals("value"), "this field should be value");
  int field_offset = ik->field_offset(value_field_index);

  typeArrayOop char_array_obj = (typeArrayOop)(str_obj->obj_field(field_offset));
}

char* ConcolicMngr::javaStringToC(oop str_obj) {
  typeArrayOop char_array_obj = charArrayObjFromJavaString(str_obj);
  
  int length = char_array_obj->length();
  char *char_array = NEW_RESOURCE_ARRAY(char, length + 1);
  char_array[length] = 0;
  for (int i = 0; i < length; i++) {
    char_array[i] = char_array_obj->char_at(i);
  }

  return char_array;
}

void ConcolicMngr::symbolizeMethod(Handle holder_name_handle, Handle callee_name_handle) {
  ResourceMark rm;
  char *holder_name = javaStringToC(holder_name_handle());
  char *callee_name = javaStringToC(callee_name_handle());

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