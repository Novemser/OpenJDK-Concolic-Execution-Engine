#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>

class ZeroFrame;

class MethodSymbolizer {
  typedef std::set<std::string> SymMethodSet;
  typedef SymMethodSet::iterator SymMethodSetIt;
  typedef std::map<std::string, SymMethodSet *> SymClassMap;
  typedef SymClassMap::iterator SymClassMapIt;

  SymClassMap _symbolicMethods;

public:
  struct Handle {
    ZeroFrame *caller_frame;
    ZeroFrame *callee_frame;
    exp_list_t param_list;
    std::string callee_holder_name;
    std::string callee_name;

    void reset();
  } _handle;

  ~MethodSymbolizer();

  void invoke_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void finish_method(ZeroFrame *caller_frame);

  void add_symbolic_method(const std::string &class_name,
                           const std::string &method_name);

  void print();

private:
  void invoke_method_helper(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void finish_method_helper(ZeroFrame *caller_frame, ZeroFrame *callee_frame);

  SymMethodSet *get_sym_methods(const std::string class_name);
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP