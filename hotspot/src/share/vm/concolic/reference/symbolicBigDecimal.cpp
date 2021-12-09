#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicBigDecimal.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"

const char *SymBigDecimal::TYPE_NAME = "java/math/BigDecimal";
bool SymBigDecimal::need_recording = false;


std::set<std::string> SymBigDecimal::handle_method_names = init_handle_method_names();

std::set<std::string> SymBigDecimal::init_handle_method_names() {
  std::set<std::string> set;
  return set;
}

std::map<std::string, bool> SymBigDecimal::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymBigDecimal::init_skip_method_names() {
  std::map<std::string, bool> map;
//  map["<init>"] = false;
  return map;
}

SymBigDecimal::SymBigDecimal(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}

SymBigDecimal::~SymBigDecimal() { Expression::gc(_exp); }

bool SymBigDecimal::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = true;
  need_recording = false;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_recording = handle.general_check_param_symbolized();
    if (need_recording) {}
    need_symbolize = true;
  } else {
    std::map<std::string, bool>::iterator iter = skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
      if (!need_symbolize) {
        bool recording = handle.general_check_param_symbolized();
        handle.get_callee_method()->print_name(tty);
        tty->print_cr(" skipped by SymBigDecimal, need recording %c", recording ? 'Y' : 'N');
      }
    } else {
      bool recording = handle.general_check_param_symbolized();
      handle.get_callee_method()->print_name(tty);
      tty->print_cr(" handled by SymBigDecimal, need recording %c", recording ? 'Y' : 'N');
    }
  }

  return need_symbolize;
}

Expression *SymBigDecimal::finish_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;
//  ShouldNotCallThis();
//  if (callee_name == "add") {
//    oop left_obj = handle.get_param<oop>(0);
//    oop right_obj = handle.get_param<oop>(1);
//    if (left_obj->is_symbolic() || right_obj->is_symbolic()) {
//      oop res_obj = handle.get_result<oop>(T_OBJECT);
//      Expression *left_exp = SymBigDecimal::get_exp_of(left_obj);
//      Expression *right_exp = SymBigDecimal::get_exp_of(right_obj);
//      SymBigDecimal *sym_res = (SymBigDecimal *)ConcolicMngr::ctx->get_or_alloc_sym_inst(res_obj);
//      exp = new OpSymExpression(left_exp, right_exp, op_add, true);
//      sym_res->set_ref_exp(exp);
//    }
//  } else {
//    ShouldNotCallThis();
//  }
  return exp;
}

Expression *SymBigDecimal::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    assert(exp != NULL, "NOT NULL");
  } else {
    ResourceMark rm;
    //TODO: get double value from BigDecimal obj;
    ShouldNotCallThis();
  }
  return exp;
}

void SymBigDecimal::print() {
  tty->print_cr("SymBigDecimal: ");
  _exp->print();
  tty->print("\n");
}

#endif