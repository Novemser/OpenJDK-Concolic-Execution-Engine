#ifdef ENABLE_CONCOLIC

#include "concolic/threadContext.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/exp/expression.hpp"
#include "concolic/fieldTraverser.hpp"
#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/reference/symbolicObject.hpp"
#include "concolic/reference/symbolicPrimitive.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/reference/symbolicStringBuilder.hpp"
#include "concolic/reference/symbolicBigDecimal.hpp"
#include "concolic/reference/symbolicTimestamp.hpp"
#include "concolic/reference/symbolicDate.hpp"
#include "concolic/jdbc/reference/symbolicHibernateKey.hpp"
#include "utilities/vmError.hpp"
#include "utilities/exceptions.hpp"
#include "concolic/reference/symbolicNDFunc.hpp"

// An object closure to reset symbolic ids
class RestoreSymbolicClosure : public ObjectClosure {
public:
  void do_object(oop o) {
    if (o != NULL) {
      o->set_sym_rid(NULL_SYM_RID);
    }
  }
};

ThreadContext::ThreadContext(JavaThread *jt) : _thread(jt), _s_stack(jt), _sym_rid_counter(), _sym_tmp_id_counter(),
                                               _path_condition_enabled(true) {
  init_sym_rid_counter();
  init_sym_tmp_id_counter();

  Expression::init_gc_helper();
  SymbolExpression::init_single_instances();
}

ThreadContext::~ThreadContext() {
  for (SymStore::iterator iter = _sym_refs.begin(); iter != _sym_refs.end();
       ++iter) {
    guarantee(iter->second != NULL, "Sym instance should not be null");
    delete iter->second;
  }
  _sym_refs.clear();

  int size = _sym_tmp_exps.size();
  for (int i = 1; i != size; ++i) {
    Expression::gc(_sym_tmp_exps[i]);
  }
  _sym_tmp_exps.clear();
  _path_condition.gc();
  ArrayStore::iterator arr_it;

  for (arr_it = _array_store.begin(); arr_it != _array_store.end(); arr_it++) {
    guarantee(arr_it->second != NULL, "Array store should not be null");
    delete arr_it->second;
  }
  SymbolicNDFunc::gc();

  RestoreSymbolicClosure rso;
  Universe::heap()->safe_object_iterate(&rso);

  Expression::finalize_dangling_objects();
  SymString::INTERN_MAP.clear();
  SymStmt::resetGlobalCounter();
}

SymInstance *ThreadContext::get_or_alloc_sym_inst(oop obj) {
  if (obj == NULL) return NULL;
  if (obj->is_symbolic()) {
    return this->get_sym_inst(obj->get_sym_rid());
  } else {
    return this->alloc_sym_inst(obj);
  }
}

SymInstance *ThreadContext::alloc_sym_inst(oop obj) {
  sym_rid_t sym_rid = get_next_sym_rid();
  SymInstance *sym_inst;
  ResourceMark rm;

  Symbol *klass_symbol = obj->klass()->name();
  if (klass_symbol == NULL) {
    guarantee(false, "unexpected null class");
    sym_inst = new SymResSet(sym_rid);
    obj->set_sym_rid(sym_rid);
    this->set_sym_ref(sym_rid, sym_inst);
//  _allocated_objs.insert(obj);
    return sym_inst;
  }
  std::string class_name(klass_symbol->as_C_string());
//  tty->print_cr("alloc_sym_inst for %s", klass_symbol->as_C_string());

  if ((klass_symbol->equals(SymString::TYPE_NAME)) || (klass_symbol->equals(SymStrBuilder::TYPE_NAME))) {
    if (klass_symbol->equals(SymString::TYPE_NAME)) {
      if (OopUtils::is_java_string_interned(obj)) {
        ResourceMark rm;
        const char *str = OopUtils::java_string_to_c(obj);
        tty->print_cr("Unsupported symbolizing behavior of interned String!: [%s]", str);
        /**
         * we do not support symbolize interned Java string
         */
        return NULL;
      }
    }
    sym_inst = new SymString(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jchar>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jchar>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jboolean>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jboolean>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jbyte>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jbyte>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jint>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jint>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jshort>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jshort>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jlong>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jlong>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<jfloat>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jfloat>(sym_rid);
  } else if (klass_symbol->equals(SymPrimitive<double>::TYPE_NAME)) {
    sym_inst = new SymPrimitive<jdouble>(sym_rid);
  } else if (klass_symbol->equals(SymBigDecimal::TYPE_NAME)) {
    sym_inst = new SymBigDecimal(sym_rid, obj);
  } else if (klass_symbol->equals(SymTimestamp::TYPE_NAME)) {
    sym_inst = new SymTimestamp(sym_rid, obj);
  } else if (klass_symbol->equals(SymbolicDate::TYPE_NAME)) {
    sym_inst = new SymbolicDate(sym_rid, obj);
  } else if (SymStmt::target(class_name)) {
    sym_inst = new SymStmt(sym_rid);
  } else if (SymResSet::target(class_name)) {
    sym_inst = new SymResSet(sym_rid);
  }
//  else if (SymHibernateKey::target(class_name)) {
//    sym_inst = new SymHibernateKey(sym_rid);
//  }
  else {
    sym_inst = new SymObj(sym_rid);
  }
  obj->set_sym_rid(sym_rid);
  this->set_sym_ref(sym_rid, sym_inst);
//  _allocated_objs.insert(obj);
  return sym_inst;
}

SymInstance *ThreadContext::get_sym_inst(sym_rid_t sym_rid) {
  SymInstance *ret = (SymInstance *) _sym_refs[sym_rid];
  /**
   * When this assertion is broken,
   * it means that the target object is not initialized with NULL_SYM_RID
   */
  guarantee(ret != NULL, "Unexpected null symbolic object. The object might not properly symbolized.");
  return ret;
}

SymArr *ThreadContext::get_or_alloc_sym_array(arrayOop array,
                                              Expression *length_exp) {
  if (array->is_symbolic()) {
    return this->get_sym_array(array->get_sym_rid());
  } else {
    return this->alloc_sym_array(array, length_exp);
  }
}

SymArr *ThreadContext::alloc_sym_array(arrayOop array, Expression *length_exp) {
  sym_rid_t sym_rid = get_next_sym_rid();
  array->set_sym_rid(sym_rid);

  // if (length_exp == NULL) {
  //   length_exp = new ConExpression(array->length());
  // }

  SymArr *sym_arr = new SymArr(sym_rid, length_exp);
  this->set_sym_ref(sym_rid, sym_arr);

  this->record_path_condition(
      new ArrayInitExpression(sym_arr->get_sym_rid(), array));
//  _allocated_objs.insert(array);
  return sym_arr;
}

SymArr *ThreadContext::get_sym_array(sym_rid_t sym_rid) {
  SymArr *ret = (SymArr *) _sym_refs[sym_rid];
  /**
   * When this assertion is broken,
   * it means that the target object is not initialized with NULL_SYM_RID
   */
  assert(ret != NULL, "null sym obj?");
  return ret;
}

void ThreadContext::symbolize_recursive(oop obj) {
//  tty->print_cr(obj->klass()->signature_name());
//  tty->print_cr(obj->klass()->name()->as_C_string());
//  SimpleFieldPrinter field_printer(obj);
//   field_printer.do_recursive();

  FieldSymbolizer field_symbolzier(obj, *this);
  field_symbolzier.do_recursive();
  // OopTraverser oop_traverser;
  // tty->print_cr(CL_CYAN"================================================================="CNONE);
  // obj->oop_iterate(&oop_traverser);
  // tty->print_cr(CL_CYAN"================================================================="CNONE);
}

void ThreadContext::symbolize_recursive(oop obj, std::string prefix) {
//  tty->print_cr(obj->klass()->signature_name());
//  tty->print_cr(obj->klass()->name()->as_C_string());
//  SimpleFieldPrinter field_printer(obj);
//   field_printer.do_recursive();

  FieldSymbolizer field_symbolzier(obj, *this, prefix);
  field_symbolzier.do_recursive();
  // OopTraverser oop_traverser;
  // tty->print_cr(CL_CYAN"================================================================="CNONE);
  // obj->oop_iterate(&oop_traverser);
  // tty->print_cr(CL_CYAN"================================================================="CNONE);
}

sym_tmp_id_t ThreadContext::get_next_sym_tmp_id(Expression *sym_exp) {
  sym_rid_t sym_tmp_id = _sym_tmp_id_counter++;
  sym_exp->inc_ref();
  _sym_tmp_exps.push_back(sym_exp);
  assert(_sym_tmp_exps.size() == sym_tmp_id + 1, "sanity check");
  assert(sym_tmp_id < MAX_SYM_RID, "sym_tmp_id limitted");
  return sym_tmp_id;
}

void ThreadContext::detach_tmp_exp(sym_tmp_id_t sym_tmp_id) {
  Expression *exp = _sym_tmp_exps[sym_tmp_id];
  if (exp) {
    exp->dec_ref();
  }
  _sym_tmp_exps[sym_tmp_id] = NULL;
}

std::string ThreadContext::get_current_code_pos() {
  static const int SIZE = 1024;
  static char buf[SIZE];

  stringStream ss(buf, SIZE);

  const ZeroFrame *zero_frame = get_shadow_stack().get_last_frame().get_zero_frame();
  while (!zero_frame->is_interpreter_frame()) {
    zero_frame = zero_frame->next();
  }
  interpreterState istate = zero_frame->as_interpreter_frame()->interpreter_state();
  address bcp = istate->bcp();

  Method *method = istate->method();
  int line = -1;
  if (bcp != NULL) {
    int bci = method->bci_from(bcp);
    line = method->line_number_from_bci(bci);
  }

  method->print_name(&ss);
  ss.print(":%d", line);

  return std::string(buf, ss.size());
}

static void print_method_name_and_line(const ZeroFrame *zero_frame, stringStream *ss) {
  interpreterState istate = zero_frame->as_interpreter_frame()->interpreter_state();
  Method *method = istate->method();
  address bcp = istate->bcp();

  method->print_name(ss);
  if (bcp) {
    int bci = method->bci_from(bcp);
    int line = method->line_number_from_bci(bci);
    ss->print(":%d", line);
  }
}

std::string ThreadContext::get_code_pos_for_first(const std::string &str) {
  static const int SIZE = 2048;
  static char buf[SIZE];

  stringStream ss(buf, SIZE);

  const char *c_str = str.c_str();
  const ZeroFrame *zero_frame = get_shadow_stack().get_last_frame().get_zero_frame();

  // generate top method name and line string
  while (!zero_frame->is_interpreter_frame()) {
    zero_frame = zero_frame->next();
  }
  print_method_name_and_line(zero_frame, &ss);
  std::string top_name(buf, ss.size());
  tty->print_cr("trying: %s", top_name.c_str());

  // if not containing, use a loop to find it
  if (!strstr(ss.as_string(), c_str)) {
    zero_frame = zero_frame->next();
    while (zero_frame) {
      while (zero_frame && !zero_frame->is_interpreter_frame()) {
        zero_frame = zero_frame->next();
      }
      if (!zero_frame) {
        break;
      }
      ss.reset();
      print_method_name_and_line(zero_frame, &ss);
      // tty->print_cr("trying: %s", ss.as_string());
      if (strstr(ss.as_string(), c_str)) {
        break;
      }
      zero_frame = zero_frame->next();
    }
  }

  return std::string(buf, ss.size());
}

void ThreadContext::print() {
//  for (SymStore::iterator sym_iter = _sym_refs.begin();
//       sym_iter != _sym_refs.end(); ++sym_iter) {
//    tty->print_cr("- sym_inst[%lu]:", sym_iter->first);
//    sym_iter->second->print();
//  }

//  int size = _sym_tmp_exps.size();
//  for (int i = 1; i < size; ++i) {
//    Expression *sym_exp = _sym_tmp_exps[i];
//    if (sym_exp) {
//      tty->print_cr("<< sym_tmp_exp[%d]:", i);
//      sym_exp->print_cr();
//    }
//  }
  _jdbc_mngr.print();

  _path_condition.print();
}

void ThreadContext::print_stack_trace() {
  static char buf[O_BUFLEN];
  VMError::print_stack_trace(tty, _thread, buf, sizeof(buf));
//  this->get_shadow_stack().print();
}

void ThreadContext::memory_leak_check() {
  tty->print_cr("Checking memory leaks for Expression, %lu remains...",
                Expression::total_count);
}

PathCondition &ThreadContext::get_path_condition() {
  return _path_condition;
}

void ThreadContext::set_pc_enabled(bool pathConditionEnabled) {
  _path_condition_enabled = pathConditionEnabled;
}

#endif