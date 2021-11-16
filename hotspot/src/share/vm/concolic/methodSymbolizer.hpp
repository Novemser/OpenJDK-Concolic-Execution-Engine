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
  typedef std::set<std::string> SymMethodSet;
  typedef SymMethodSet::iterator SymMethodSetIt;
  typedef std::map<std::string, SymMethodSet*> SymClassMap;
  typedef SymClassMap::iterator SymClassMapIt;

private:
  SymClassMap _symbolicMethods;
  ZeroFrame *_frame;
  ParamList _param_list;

  std::string _callee_holder_name_string;
  std::string _callee_name_string;

public:
  ~MethodSymbolizer();
  
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