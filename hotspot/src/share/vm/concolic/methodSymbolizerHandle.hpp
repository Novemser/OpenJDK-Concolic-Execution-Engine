#ifndef SHARE_VM_CONCOLIC_METHODSYMBOLIZERHANDLE_HPP
#define SHARE_VM_CONCOLIC_METHODSYMBOLIZERHANDLE_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/exp/expression.hpp"
#include "interpreter/bytecodeInterpreter.hpp"
#include "interpreterFrame_zero.hpp"
#include "runtime/frame.hpp"
#include "runtime/frame.inline.hpp"

#include <string>

class MethodSymbolizerHandle {
private:
  ZeroFrame *_caller_frame;
  ZeroFrame *_callee_frame;
  interpreterState _callee_istate;
  exp_list_t _param_list;
  std::string _callee_holder_name;
  std::string _callee_name;

public:
  /**
   * returns the being offset of first parameter in callee's local variable
   * table
   */
  inline int get_callee_local_begin_offset() { return 0; }

  int get_caller_stack_begin_offset() {
    if (_caller_frame->is_interpreter_frame()) {
      interpreterState caller_istate = _caller_frame->as_interpreter_frame()->interpreter_state();
      return caller_istate->stack_base() - _callee_istate->locals() - 1;
    } else if (_caller_frame->is_entry_frame()) {
      return 0;
    }
    ShouldNotCallThis();
    return 0;
  }

  /**
   *!!the offset returned is error, discard now.
   */
  // inline int get_end_offset() {
  //   return _caller_istate->stack_base() - _caller_istate->stack() - 1;
  // }
  void sanityCheck() {
    Method *callee_method = _callee_istate->method();
    if (_caller_frame->is_interpreter_frame()) {
      ResourceMark rm;
      interpreterState caller_istate = _caller_frame->as_interpreter_frame()->interpreter_state();
      char *callee_method_name_c_str = callee_method->method_holder()->name()->as_C_string();
      // for lambda expressions(where the call site might locate in java/lang/invoke),
      // caller_istate->callee() == callee_method might not hold
      if (!strstr(callee_method_name_c_str, "java/lang/invoke") &&
          !strstr(callee_method_name_c_str, "$$Lambda")) {
        assert(caller_istate->callee() == callee_method, "should be");
      }
    } else if (_callee_frame->is_entry_frame()) {
      JavaCallWrapper *call_wrapper = *_caller_frame->as_entry_frame()->call_wrapper();
      assert(call_wrapper->callee_method() == callee_method, "should be");
    }
  }

  Method *get_callee_method() {
    sanityCheck();
    return _callee_istate->method();
  }

  inline intptr_t *get_locals_ptr() { return _callee_istate->locals(); }

  BasicType get_result_type() {
    Method *callee_method = NULL;
    if (_caller_frame->is_interpreter_frame()) {
      callee_method = _caller_frame->as_interpreter_frame()->interpreter_state()->callee();
    } else if (_caller_frame->is_entry_frame()) {
      JavaCallWrapper *call_wrapper = *_caller_frame->as_entry_frame()->call_wrapper();
      callee_method = call_wrapper->callee_method();
    }
    return callee_method->result_type();
  }

  template<class T>
  T get_result(BasicType type) {
    int result_slots = type2size[type];
    assert(result_slots >= 0 && result_slots <= 2, "what?");
    return *(T *) (_callee_istate->stack() + result_slots);
  }

  inline void set_caller_frame(ZeroFrame *caller_frame) {
    _caller_frame = caller_frame;
  }

  inline void set_callee_frame(ZeroFrame *callee_frame) {
    _callee_frame = callee_frame;
    _callee_istate = callee_frame->as_interpreter_frame()->interpreter_state();
  }

  inline ZeroFrame *get_caller_frame() { return _caller_frame; }

  inline ZeroFrame *get_callee_frame() { return _callee_frame; }

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

  template<class T>
  inline T get_param(int offset) {
    return *(T *) (get_locals_ptr() - offset);
  }

  Expression *get_primitive_exp(int offset, BasicType type);

  void general_prepare_param(int max_param_num = 100);

  int general_prepare_param_helper(BasicType type, int locals_offset, bool is_this);

  bool general_check_param_symbolized();

  bool general_check_param_symbolized_helper(BasicType type, int &locals_offset);

  Expression *general_prepare_result_helper();

public:
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
    _callee_istate = NULL;
    clear_param_list();
    _callee_holder_name.clear();
    _callee_name.clear();
  }

};

#endif

#endif // SHARE_VM_CONCOLIC_METHODSYMBOLIZERHANDLE_HPP