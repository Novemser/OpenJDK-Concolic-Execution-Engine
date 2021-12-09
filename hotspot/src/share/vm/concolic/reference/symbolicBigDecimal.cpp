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
  set.insert("valueOf");
  set.insert("setScale");
  set.insert("equals");
  set.insert("add");
  set.insert("multiply");
  set.insert("subtract");
  set.insert("divide");
  set.insert("compareTo");
  return set;
}

std::map<std::string, bool> SymBigDecimal::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymBigDecimal::init_skip_method_names() {
  std::map<std::string, bool> map;
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
    if (need_recording) {
      handle.general_prepare_param();
    }
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
  if (!need_recording) {
    return NULL;
  }
  const std::string &callee_name = handle.get_callee_name();
  Expression *exp = NULL;

  if (callee_name == "valueOf") {
    oop res_obj = handle.get_result<oop>(T_OBJECT);
    SymBigDecimal *sym_bigd = (SymBigDecimal *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);
    Expression *value_exp = handle.get_param_list()[0];
    sym_bigd->set_ref_exp(value_exp);
  } else if (callee_name == "setScale") {
    oop res_obj = handle.get_result<oop>(T_OBJECT);
    SymBigDecimal *sym_bigd = (SymBigDecimal *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);

    Expression *this_exp = handle.get_param_list()[0];
    guarantee(this_exp != NULL, "should be");
    sym_bigd->set_ref_exp(this_exp);
  } else {
    Expression *left_exp = handle.get_param_list()[0];
    Expression *right_exp = handle.get_param_list()[1];
    BasicType res_type = handle.get_result_type();

    if (res_type == T_OBJECT) {
      oop res_obj = handle.get_result<oop>(T_OBJECT);
      SymBigDecimal *sym_res = (SymBigDecimal *)ConcolicMngr::ctx->get_or_alloc_sym_inst(res_obj);
      Expression* res_exp = NULL;
      if (callee_name == "add") {
        res_exp = new OpSymExpression(left_exp, right_exp, op_add);
      } else if (callee_name == "subtract") {
        res_exp = new OpSymExpression(left_exp, right_exp, op_sub);
      } else if (callee_name == "multiply") {
        res_exp = new OpSymExpression(left_exp, right_exp, op_mul);
      } else if (callee_name == "divide") {
        res_exp = new OpSymExpression(left_exp, right_exp, op_div);
      }
      sym_res->set_ref_exp(res_exp);
    }
    //      oop res_obj = handle.get_result<oop>(T_OBJECT);
    if (callee_name == "equals") {
      exp = new OpSymExpression(left_exp, right_exp, op_eq);
    } else if (callee_name == "compareTo") {
      exp = new OpSymExpression(left_exp, right_exp, op_cmp);
    }
  }

  return exp;
}

Expression *SymBigDecimal::get_exp_of(oop obj) {
  assert(obj->klass()->name()->equals(TYPE_NAME), "should be");
  Expression *exp;
  if (obj->is_symbolic()) {
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    exp = sym_inst->get_ref_exp();
    guarantee(exp != NULL, "NOT NULL");
  } else {
    exp = new InstanceSymbolExp(obj);
  }
  return exp;
}

void SymBigDecimal::print() {
  tty->print_cr("SymBigDecimal: ");
  _exp->print();
  tty->print("\n");
}

#endif