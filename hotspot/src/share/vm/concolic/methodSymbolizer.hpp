#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <vector>
#include <map>
#include <set>
#include <string>

class ZeroFrame;

class MethodSymbolizer {
  typedef std::vector<Expression *> ParamList;

private:
  std::set<std::string> _symbolicMethods;
  ZeroFrame *_frame;
  ParamList _param_list;

public:
  void invoke_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void finish_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);

private:
  void invoke_method_helper(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void reset();
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP