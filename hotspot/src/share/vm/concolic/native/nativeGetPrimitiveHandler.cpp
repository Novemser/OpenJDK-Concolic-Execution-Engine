#ifdef ENABLE_CONCOLIC

#include "nativeGetPrimitiveHandler.hpp"

std::set<std::string> NativeGetPrimitiveHandler::handle_method_name_sigs = init_handle_method_name_sigs();

std::set<std::string> NativeGetPrimitiveHandler::init_handle_method_name_sigs() {
  std::set<std::string> name_sigs;
  name_sigs.insert(std::string("sun.misc.Unsafe.getByte(Ljava/lang/Object;J)B"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getChar(Ljava/lang/Object;J)C"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getDouble(Ljava/lang/Object;J)D"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getFloat(Ljava/lang/Object;J)F"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getInt(Ljava/lang/Object;J)I"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getLong(Ljava/lang/Object;J)J"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getShort(Ljava/lang/Object;J)S"));
  name_sigs.insert(std::string("sun.misc.Unsafe.getBoolean(Ljava/lang/Object;J)Z"));

  return name_sigs;
}

bool NativeGetPrimitiveHandler::target(const std::string& name_sig) {
  return handle_method_name_sigs.find(name_sig) != handle_method_name_sigs.end();
}

void NativeGetPrimitiveHandler::handle_native(intptr_t *locals, BasicType result_type) {
  oop obj = reinterpret_cast<oop&>(locals[-1]);
  jlong offset = (jlong)locals[-3];

  if (obj->is_symbolic()) {
    int stack_offset = type2size[result_type] - 1;
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    Expression *exp = sym_inst->get(offset);
    ConcolicMngr::ctx->set_stack_slot(stack_offset, exp);
  }
}

#endif