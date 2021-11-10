#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/shadow/shadowStack.hpp"
#include "concolic/instance/symbolicInstance.hpp"
#include "concolic/instance/symbolicObject.hpp"
#include "concolic/pathCondition.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/handles.hpp"

#include <map>
#include <vector>

class ThreadContext {
  /**
   * TODO: make this hashtable!
   */
  typedef std::map<sym_oid_t, SymInstance *> SymStore;
  typedef std::vector<Expression *> SymTmpExpStore;

private:
  JavaThread *_thread;
  ShadowStack _s_stack;
  SymStore _sym_objs;
  SymTmpExpStore _sym_tmp_exps;
  PathCondition _path_condition;

  sym_oid_t _sym_oid_counter;
  sym_tmp_id_t _sym_tmp_id_counter;

public:
  ThreadContext(JavaThread *jt);
  ~ThreadContext();

  ShadowStack &get_shadow_stack() { return _s_stack; }

  void symbolize(Handle handle);

  SymObj *get_or_alloc_sym_obj(oop obj);
  SymObj *alloc_sym_obj(oop obj);
  SymObj *alloc_sym_array(arrayOop array);
  SymObj *get_sym_obj(sym_oid_t sym_oid);
  SymObj *get_sym_arr(sym_oid_t sym_oid);

  sym_tmp_id_t get_next_sym_tmp_id(Expression *sym_exp);
  void detach_tmp_exp(sym_tmp_id_t sym_tmp_id);

  inline void record_path_condition(Expression *sym_exp) {
    _path_condition.add(sym_exp);
  }
private:
  void symbolize_recursive(SymObj *sym_obj, oop obj);

  void set_sym_obj(sym_oid_t sym_oid, SymObj *sym_obj) {
    _sym_objs.insert(std::make_pair(sym_oid, sym_obj));
  }

  sym_oid_t get_next_sym_oid() {
    sym_oid_t ret = _sym_oid_counter++;
    assert(ret < MAX_SYM_OID, "sym_oid limitted");
    return ret;
  }

  inline void init_sym_oid_counter() { _sym_oid_counter = 1; }
  inline void init_sym_tmp_id_counter() {
    /** 
     * tmp_id start from 1. 
     * Therefore, we push a placeholder.
     */
    _sym_tmp_exps.push_back(NULL);
    _sym_tmp_id_counter = 1;
  }

public:
  void print();
  void print_stack_trace();
};

#endif

#endif // SHARE_VM_CONCOLIC_THREADCONTEXT_HPP