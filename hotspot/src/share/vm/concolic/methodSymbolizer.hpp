#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <vector>

class ZeroFrame;

class MethodSymbolizer {
  typedef std::vector<Expression *> ParamList;
private:
	ZeroFrame* _frame;
	ParamList _param_list;
public:
  void invoke_method(ZeroFrame *caller_frame);
  void finish_method(ZeroFrame *caller_frame);

private:
	void reset();
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP