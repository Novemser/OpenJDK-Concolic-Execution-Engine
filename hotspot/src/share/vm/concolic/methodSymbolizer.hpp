#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

class Method;

class MethodSymbolizer {
public:
  void invoke_method(Method *method);
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP