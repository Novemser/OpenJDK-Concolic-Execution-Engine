#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/pathCondition.hpp"
#include "concolic/reference/symbolicArray.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/shadow/shadowStack.hpp"
#include "oops/oop.inline.hpp"
#include "runtime/handles.hpp"

#include <map>
#include <vector>

class ThreadContext {
  /**
   * TODO: make this hashtable!
   */
  typedef std::map<sym_rid_t, SymRef *> SymStore;
  typedef std::vector<Expression *> SymTmpExpStore;

private:
  JavaThread *_thread;
  ShadowStack _s_stack;
  SymStore _sym_refs;
  SymTmpExpStore _sym_tmp_exps;
  PathCondition _path_condition;

  sym_rid_t _sym_rid_counter;
  sym_tmp_id_t _sym_tmp_id_counter;

public:
  inline JavaThread *get_thread() { return _thread; }

public:
  ThreadContext(JavaThread *jt);
  ~ThreadContext();

  ShadowStack &get_shadow_stack() { return _s_stack; }

  void symbolize(Handle handle);

  SymInstance *get_or_alloc_sym_inst(oop obj);
  SymInstance *alloc_sym_inst(oop obj);
  inline SymInstance *get_sym_inst(oop obj) {
    return get_sym_inst(obj->get_sym_rid());
  }

  SymInstance *get_sym_inst(sym_rid_t sym_rid);

  SymArr *get_or_alloc_sym_array(arrayOop array, Expression *length_exp = NULL);
  SymArr *alloc_sym_array(arrayOop array, Expression *length_exp = NULL);
  SymArr *get_sym_array(sym_rid_t sym_rid);

  sym_tmp_id_t get_next_sym_tmp_id(Expression *sym_exp);
  void detach_tmp_exp(sym_tmp_id_t sym_tmp_id);

  inline void record_path_condition(Expression *sym_exp) {
    _path_condition.add(sym_exp);
  }

public:
  /**
   * this will generate a new tmp_id; use it when `sym_exp` is newly
   * calculated, which doesn't have `sym_rid` and `index`
   */
  void set_stack_slot(int offset, Expression *sym_exp);
  void set_stack_slot(int offset, Expression *sym_exp, sym_rid_t sym_rid,
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

  void set_sym_ref(sym_rid_t sym_rid, SymRef *sym_ref) {
    _sym_refs.insert(std::make_pair(sym_rid, sym_ref));
  }

  sym_rid_t get_next_sym_rid() {
    sym_rid_t ret = _sym_rid_counter++;
    assert(ret < MAX_SYM_RID, "sym_rid limitted");
    return ret;
  }

  inline void init_sym_rid_counter() { _sym_rid_counter = 1; }
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