#ifndef SHARE_VM_CONCOLIC_THREADCONTEXT_HPP
#define SHARE_VM_CONCOLIC_THREADCONTEXT_HPP

#ifdef ENABLE_CONCOLIC

#include "concolic/jdbc/jdbcMngr.hpp"
#include "concolic/methodSymbolizer.hpp"
#include "concolic/pathCondition.hpp"
#include "concolic/reference/symbolicArray.hpp"
#include "concolic/reference/symbolicInstance.hpp"
#include "concolic/reference/array/arrayInternal.hpp"
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
  typedef std::map<arrayOop, ArrayInternal *> ArrayStore;

private:
  JavaThread *_thread;
  ShadowStack _s_stack;
  SymStore _sym_refs;
  SymTmpExpStore _sym_tmp_exps;
  PathCondition _path_condition;
  MethodSymbolizer _method_symbolizer;
  JdbcMngr _jdbc_mngr;
  ArrayStore _array_store;

  sym_rid_t _sym_rid_counter;
  sym_tmp_id_t _sym_tmp_id_counter;

  bool _path_condition_enabled;
//  std::set<oop> _allocated_objs;
public:
  ThreadContext(JavaThread *jt);
  ~ThreadContext();

  inline JavaThread *get_thread() { return _thread; }

  inline void symbolize(Handle handle) { this->symbolize_recursive(handle()); }
  inline void symbolize(Handle handle, std::string prefix) { this->symbolize_recursive(handle(), prefix); }
  inline void symbolize_method(const char *class_name,
                               const char *method_name) {
    _method_symbolizer.add_method(class_name, method_name);
  }
  inline void record_stmt_obj(oop stmt, oop obj) {
    _jdbc_mngr.record_stmt_obj_pair(stmt, obj);
  }
  inline void record_persistent_obj(oop obj) {
    _jdbc_mngr.record_persistent_obj(obj);
  }

  PathCondition &get_path_condition();

  void set_pc_enabled(bool pathConditionEnabled);

public:
  void __attribute__((optimize("O0"))) printSymExp(oop obj) {
    obj->print();
    tty->print_cr("is symbolic: %d", (int)obj->is_symbolic());
    if (obj->is_symbolic()) {
      SymInstance* sym_inst = this->get_sym_inst(obj);
      sym_inst->print();
    }
  }

  inline JdbcMngr &get_jdbc_mngr() { return _jdbc_mngr; }

  inline MethodSymbolizer &get_method_symbolizer() {
    return _method_symbolizer;
  }

  inline bool is_symbolizing_method() {
    _method_symbolizer.is_symbolizing_method();
  }

  inline const ArrayStore &get_array_store() const {
    return _array_store;
  }

  inline bool has_callback() const {
    return _method_symbolizer.has_callback();
  }

public:
  std::string get_current_code_pos();
  std::string get_code_pos_for_first(const std::string &str);

public:
  SymInstance *get_or_alloc_sym_inst(oop obj);
  SymInstance *alloc_sym_inst(oop obj);
  inline SymInstance *get_sym_inst(oop obj) {
    return get_sym_inst(obj->get_sym_rid());
  }

  SymInstance *get_sym_inst(sym_rid_t sym_rid);

  SymArr *get_or_alloc_sym_array(arrayOop array, Expression *length_exp = NULL);
  SymArr *alloc_sym_array(arrayOop array, Expression *length_exp = NULL);
  SymArr *get_sym_array(sym_rid_t sym_rid);
  inline SymArr *get_sym_array(arrayOop array_obj) {
    return get_sym_array(array_obj->get_sym_rid());
  }

  sym_tmp_id_t get_next_sym_tmp_id(Expression *sym_exp);
  void detach_tmp_exp(sym_tmp_id_t sym_tmp_id);

  inline void __attribute__((optimize("O0"))) record_path_condition(Expression *sym_exp) {
    if (!_path_condition_enabled) {
      return;
    }

    _path_condition.add(sym_exp);
  }

  ArrayInternal* get_or_create_array_internal(arrayOop oop);
public:
  inline ShadowStack &get_shadow_stack() { return _s_stack; }
  /**
   * this will generate a new tmp_id; use it when `sym_exp` is newly
   * calculated, which doesn't have `sym_rid` and `index`
   */
  void set_stack_slot(int offset, Expression *sym_exp);
  void set_stack_slot_if_possible(int offset, Expression *sym_exp);
  void set_stack_slot(int offset, Expression *sym_exp, sym_rid_t sym_rid,
                      int index);

  inline Expression *get_stack_slot(int offset) {
    return this->get_stack_entry(offset).exp;
  }

  inline Expression *get_and_detach_stack_slot(int offset) {
    return this->get_and_detach_stack_entry(offset).exp;
  }

  inline Expression *get_local_slot(int offset) {
    return this->get_last_local().get_slot(offset);
  }

  inline void set_local_slot(int offset, Expression *exp) {
    this->get_last_local().set_slot(offset, exp);
  }

  /**
   * TODO: document when to clear the stack
   */
  void clear_stack_slot(int offset);
  void copy_stack_slot(int src_off, int dst_off, int size);
  void swap_two_stack_slot(int off1, int off2);
  void copy_entry_from_local_to_stack(int local_offset, int stack_offset);
  void copy_entry_from_stack_to_local(int stack_offset, int local_offset);
  void symbolize_recursive(oop obj);
  void symbolize_recursive(oop obj, std::string prefix);

private:
  inline ShadowTable &get_last_stack() {
    return this->get_shadow_stack().get_last_frame().get_opr_stack();
  }

  inline ShadowTable &get_last_local() {
    return this->get_shadow_stack().get_last_frame().get_local_tbl();
  }

  inline ShadowTable::Entry &get_stack_entry(int offset) {
    return this->get_last_stack().get_entry(offset);
  }

  inline ShadowTable::Entry &get_local_entry(int offset) {
    return this->get_last_local().get_entry(offset);
  }

  inline ShadowTable::Entry &get_and_detach_stack_entry(int offset) {
    ShadowTable::Entry &entry = this->get_last_stack().get_entry(offset);
    if (entry.exp) {
      if (entry.sym_rid == NULL_SYM_RID) {
        this->detach_tmp_exp(entry.index);
      }
    }
    return entry;
  }

private:

  void set_sym_ref(sym_rid_t sym_rid, SymRef *sym_ref) {
    _sym_refs.insert(std::make_pair(sym_rid, sym_ref));
  }

  sym_rid_t get_next_sym_rid() {
    sym_rid_t ret = _sym_rid_counter++;
    guarantee(ret < MAX_SYM_RID, "sym_rid limitted");
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
  static void memory_leak_check();
};

#endif

#endif // SHARE_VM_CONCOLIC_THREADCONTEXT_HPP