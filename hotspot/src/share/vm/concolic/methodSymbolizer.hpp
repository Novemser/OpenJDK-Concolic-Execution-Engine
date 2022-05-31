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
  typedef std::map<std::string, bool(*)(MethodSymbolizerHandle&)> invoke_helper_method_map_t;
  typedef std::map<std::string, Expression*(*)(MethodSymbolizerHandle&)> finish_helper_method_map_t;

  bool _symbolizing_method;
  std::vector<MethodSymbolizerHandle> handling_methods;
  SymClassMap _symbolicMethods;
  MethodSymbolizerHandle _handle;
  invoke_helper_method_map_t _invoke_helper_methods;
  finish_helper_method_map_t _finish_helper_methods;
  std::vector<std::string> _classes_prefix_to_symbolize;

#ifdef ENABLE_WEBRIDGE
public:
  bool _has_callbacks;
  typedef std::map<std::string, void (*)(MethodSymbolizerHandle &)> method_exit_callback_map_t;
  method_exit_callback_map_t _method_exit_callback_methods;

  void method_exit(ZeroFrame *caller_frame);

  void add_method_exit_callback(const std::string class_name, void(*method_exit_callback)(MethodSymbolizerHandle &));

#endif

public:
  MethodSymbolizer();
  ~MethodSymbolizer();


  /* let compiler not optimize this  code!! */
  inline bool __attribute__((optimize("O0"))) is_symbolizing_method() { return _symbolizing_method; }

  inline void set_symbolizing_method(bool symbolizing_method) { _symbolizing_method = symbolizing_method; }

  bool has_callback() const;

  void set_has_callback(bool shouldProcessFinishMethod);

  void add_method(const char *class_name, const char *method_name);
  void invoke_method(ZeroFrame *caller_frame, ZeroFrame *callee_frame);
  void finish_method(ZeroFrame *caller_frame);
  // only for those are not symbolized method
  bool has_handling_methods();
  void finish_handling_method(ZeroFrame *caller_frame);

  void print();

private:
  SymMethodSet *get_sym_methods(const std::string &class_name);
  void init_helper_methods();

public:
  static void invoke_method_helper(MethodSymbolizerHandle &handle);
  static Expression *finish_method_helper(MethodSymbolizerHandle &handle);

  static int prepare_param(MethodSymbolizerHandle &handle, BasicType type,
                           intptr_t *locals, int locals_offset);
  void add_invoke_helper_methods(const std::string class_name, bool(*invoke_helper_func)(MethodSymbolizerHandle&));
  void add_finish_helper_methods(const std::string class_name, Expression*(*finish_helper_func)(MethodSymbolizerHandle&));

  void add_prefix_symbolic_class(const std::string class_name);
};

class MethodReturnSymbolExp : public SymbolExpression {
  static sym_rid_t sym_method_count;

public:
  MethodReturnSymbolExp(BasicType type);
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZER_HPP