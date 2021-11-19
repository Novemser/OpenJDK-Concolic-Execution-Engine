#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZERHANDLE_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZERHANDLE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "interpreter/bytecodeInterpreter.hpp"
#include "interpreterFrame_zero.hpp"
#include "runtime/frame.hpp"

#include <string>

class MethodSymbolizerHandle {
private:
  ZeroFrame *_caller_frame;
  ZeroFrame *_callee_frame;
  interpreterState _caller_istate;
  interpreterState _callee_istate;
  exp_list_t _param_list;
  std::string _callee_holder_name;
  std::string _callee_name;

public:
  /**
   * returns the being/end offset of first parameter in caller's oprand stack
   */
  inline int get_begin_offset() { return 0; }
  /**
   *!!the offset returned is error, discard now.
   */
  // inline int get_end_offset() {
  //   return _caller_istate->stack_base() - _caller_istate->stack() - 1;
  // }

  inline Method *get_callee_method() { return _callee_istate->method(); }

  inline intptr_t *get_locals_ptr() {
    return ((intptr_t *)_callee_istate->locals());
  }

  inline BasicType get_result_type() {
    return _caller_istate->callee()->result_type();
  }

  inline void set_caller_frame(ZeroFrame *caller_frame) {
    _caller_frame = caller_frame;
    _caller_istate = caller_frame->as_interpreter_frame()->interpreter_state();
  }

  inline void set_callee_frame(ZeroFrame *callee_frame) {
    _callee_frame = callee_frame;
    _callee_istate = callee_frame->as_interpreter_frame()->interpreter_state();
  }

  inline ZeroFrame *get_caller_frame() { return _caller_frame; }
  inline ZeroFrame *get_callee_frame() { return _callee_frame; }
  inline interpreterState get_caller_istate() { return _caller_istate; }
  inline interpreterState get_callee_istate() { return _callee_istate; }

  inline void set_callee_holder_name(const char *c_str) {
    _callee_holder_name = std::string(c_str);
  }

  inline const std::string &get_callee_holder_name() {
    return _callee_holder_name;
  }

  inline void set_callee_name(const char *c_str) {
    _callee_name = std::string(c_str);
  }

  inline const std::string &get_callee_name() { return _callee_name; }

  inline exp_list_t &get_param_list() { return _param_list; }

  inline void clear_param_list() {
    int size = _param_list.size();
    for (int i = 0; i < size; ++i) {
      Expression *exp = _param_list[i];
      if (exp && exp->able_to_gc()) {
        delete exp;
      }
    }
    _param_list.clear();
  }

  inline void reset() {
    _caller_frame = NULL;
    _callee_frame = NULL;
    _caller_istate = NULL;
    _callee_istate = NULL;
    clear_param_list();
    _callee_holder_name.clear();
    _callee_name.clear();
  }
};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZERHANDLE_HPP