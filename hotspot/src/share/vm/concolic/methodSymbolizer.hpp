#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/symbolExpression.hpp"
#include "concolic/methodSymbolizerHandle.hpp"

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

  bool _symbolizing_method;
  SymClassMap _symbolicMethods;
  MethodSymbolizerHandle _handle;

public:
  MethodSymbolizer() : _symbolizing_method(false) {}
  ~MethodSymbolizer();


  /* let compiler not optimize this  code!! */
  inline bool __attribute__((optimize("O0"))) is_symbolizing_method() { return _symbolizing_method; }

  void add_method(const char *class_name, const char *method_name);
  void invoke_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void finish_method(ZeroFrame *caller_frame);

  void print();

private:
  SymMethodSet *get_sym_methods(const std::string &class_name);

public:
  static void invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static int prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                           intptr_t *locals, int locals_offset);
};

class MethodReturnSymbolExp : public SymbolExpression {
  static sym_rid_t sym_method_count;

public:
  MethodReturnSymbolExp();
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP