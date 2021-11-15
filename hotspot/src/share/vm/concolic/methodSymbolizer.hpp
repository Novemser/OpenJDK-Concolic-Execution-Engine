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
  typedef std::set<std::string> MethodSet;
  typedef MethodSet::iterator MethodSetIt;
  typedef std::map<std::string, MethodSet*> ClassMap;
  typedef ClassMap::iterator ClassMapIt;

private:
  ClassMap _symbolicMethods;
  ZeroFrame *_frame;
  ParamList _param_list;

  std::string _callee_holder_name_string;
  std::string _callee_name_string;

public:
  // TODO: deconstructor to release memory
  void invoke_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void finish_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);

  void add_symbolic_method(std::string class_name, std::string method_name);
  bool is_symbolic_method(std::string class_name, std::string method_name);

  void print();

private:
  void invoke_method_helper(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void reset();
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP