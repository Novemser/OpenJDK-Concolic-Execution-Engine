#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/instance/symbolicArray.hpp"
#include "concolic/instance/symbolicObject.hpp"
#include "concolic/pathCondition.hpp"
#include "concolic/shadow/shadowStack.hpp"
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
  SymStore _sym_instances;
  SymTmpExpStore _sym_tmp_exps;
  PathCondition _path_condition;

  sym_oid_t _sym_oid_counter;
  sym_tmp_id_t _sym_tmp_id_counter;

public:
  inline JavaThread *get_thread() { return _thread; }

public:
  ThreadContext(JavaThread *jt);
  ~ThreadContext();

  ShadowStack &get_shadow_stack() { return _s_stack; }

  void symbolize(Handle handle);

  SymObj *get_or_alloc_sym_obj(oop obj);
  SymObj *alloc_sym_obj(oop obj);
  SymObj *get_sym_obj(sym_oid_t sym_oid);

  SymArr *get_or_alloc_sym_array(arrayOop array, Expression *length_exp = NULL);
  SymArr *alloc_sym_array(arrayOop array, Expression *length_exp = NULL);
  SymArr *get_sym_array(sym_oid_t sym_oid);

  sym_tmp_id_t get_next_sym_tmp_id(Expression *sym_exp);
  void detach_tmp_exp(sym_tmp_id_t sym_tmp_id);

  inline void record_path_condition(Expression *sym_exp) {
    _path_condition.add(sym_exp);
  }

public:
  /**
   * this will generate a new tmp_id; use it when `sym_exp` is newly
   * calculated, which doesn't have `sym_oid` and `index`
   */
  void set_stack_slot(int offset, Expression *sym_exp);
  void set_stack_slot(int offset, Expression *sym_exp, sym_oid_t sym_oid,
                      int index);
  Expression *get_stack_slot(int offset);
  Expression *get_stack_slot_and_detach(int offset);
  /**
   * TODO: document when to clear the stack
   */
  void clear_stack_slot(int offset);
  void copy_stack_slot(int src_off, int dst_off, int size);
  void swap_two_stack_slot(int off1, int off2);
  void copy_entry_from_local_to_stack(int local_offset, int stack_offset);
  void copy_entry_from_stack_to_local(int stack_offset, int local_offset);
  Expression *get_local_slot(int offset);
  void set_local_slot(int offset, Expression *sym_exp);

private:
  void symbolize_recursive(oop obj);

  void set_sym_instance(sym_oid_t sym_oid, SymInstance *sym_instance) {
    _sym_instances.insert(std::make_pair(sym_oid, sym_instance));
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