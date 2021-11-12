#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

class ZeroFrame;

class MethodSymbolizer {
public:
  void invoke_method(ZeroFrame* caller_frame);
  void finish_method(ZeroFrame* caller_frame);
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP