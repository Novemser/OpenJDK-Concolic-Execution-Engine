#ifdef ENABLE_CONCOLIC

#include "concolic/reference/symbolicBigDecimal.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/utils.hpp"


#ifdef ENABLE_WEBRIDGE

#include "concolic/reference/symbolicString.hpp"
#include "concolic/exp/stringExpression.hpp"
#include "webridge/utils/rapidjson/document.h"

#include <map>

std::string SymBigDecimal::DECIMAL_WRAPPER = "$BIG_DECIMAL$";
#endif
const char *SymBigDecimal::TYPE_NAME = "java/math/BigDecimal";
bool SymBigDecimal::need_recording = false;


std::set<std::string> SymBigDecimal::handle_method_names = init_handle_method_names();

std::set<std::string> SymBigDecimal::init_handle_method_names() {
  std::set<std::string> set;
#ifdef ENABLE_WEBRIDGE
  set.insert("java.math.BigDecimal.<init>(D)V");
  set.insert("java.math.BigDecimal.<init>(Ljava/lang/String;)V");
  set.insert("java.math.BigDecimal.valueOf(J)Ljava/math/BigDecimal;");
  set.insert("java.math.BigDecimal.valueOf(JII)Ljava/math/BigDecimal;");
  set.insert("java.math.BigDecimal.valueOf(Ljava/math/BigInteger;II)Ljava/math/BigDecimal;");
#else
  set.insert("<init>");
  set.insert("valueOf");
  set.insert("equals");
  set.insert("setScale");
  set.insert("add");
  set.insert("multiply");
  set.insert("subtract");
  set.insert("divide");
  set.insert("compareTo");
#endif
  return set;
}

std::map<std::string, bool> SymBigDecimal::skip_method_names = init_skip_method_names();

std::map<std::string, bool> SymBigDecimal::init_skip_method_names() {
  std::map<std::string, bool> map;
  return map;
}

void SymBigDecimal::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymBigDecimal::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymBigDecimal::TYPE_NAME, finish_method_helper);
  m_symbolizer->add_method_exit_callback(SymBigDecimal::TYPE_NAME, method_exit_callback);
}

SymBigDecimal::SymBigDecimal(sym_rid_t sym_rid) : SymInstance(sym_rid), _exp(NULL) {}

SymBigDecimal::~SymBigDecimal() {
  Expression::gc(_exp);
#ifdef ENABLE_WEBRIDGE
  std::map<int, Expression *>::iterator iter = _internal_fields.begin();
  while (iter != _internal_fields.end()) {
    iter++;
    Expression::gc((*iter).second);
  }
#endif
}

bool SymBigDecimal::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  need_recording = false;
  tty->print_cr("SymBigDecimal::invoke_method_helper:%s", handle.get_callee_name().c_str());

#ifdef ENABLE_WEBRIDGE
  if (handle_method_names.find(handle.get_callee_method()->name_and_sig_as_C_string()) != handle_method_names.end()) {
    need_recording = handle.general_check_param_symbolized();
    if (need_recording) {
      handle.general_prepare_param();
    }
    return true;
  }
#else
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
#endif
  return need_symbolize;
}

Expression *SymBigDecimal::finish_method_helper(MethodSymbolizerHandle &handle) {
#ifdef ENABLE_WEBRIDGE
  ResourceMark rm;
  std::string signature = handle.get_callee_method()->name_and_sig_as_C_string();
  ConcolicMngr::ctx->get_method_symbolizer().set_has_callback(false);
  if (signature == "java.math.BigDecimal.<init>(Ljava/lang/String;)V") {
    oop paramStr = handle.get_param<oop>(1);
    assert(paramStr->klass()->name()->equals(SymString::TYPE_NAME), "should be string");
    if (!paramStr->is_symbolic()) {
      return NULL;
    }
    SymString *symStr = reinterpret_cast<SymString *>(ConcolicMngr::ctx->get_sym_inst(paramStr));
    oop thisDecimal = handle.get_param<oop>(0);
    SymBigDecimal *symObj = reinterpret_cast<SymBigDecimal *>(
        ConcolicMngr::ctx->get_or_alloc_sym_inst(thisDecimal)
    );
    Expression *refExp = symStr->get_ref_exp();

    guarantee(refExp != NULL, "Should not be null");
    if (refExp->is_op_str_expression()) {
      OpStrExpression *opExp = reinterpret_cast<OpStrExpression *>(refExp);
      if (opExp->get_name() == "valueOf") {
        guarantee(opExp->get_param_list().size() == 1, "Should contain only 1 parameter");
        // inner expression
        refExp = opExp->get_param_list()[0];
      } else {
        tty->print_cr("%s not implemented in sym big decimal conversion", opExp->get_name().c_str());
        ShouldNotCallThis();
      }
    }

    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    refExp->serialize(writer);
    rapidjson::Document doc;
    doc.Parse(s.GetString());
    rapidjson::Value &name = doc["_exp"];
    symObj->set_bigDecimal_symbolic(thisDecimal, name.GetString());
  } else if (signature == "java.math.BigDecimal.valueOf(J)Ljava/math/BigDecimal;" ||
             signature == "java.math.BigDecimal.valueOf(JII)Ljava/math/BigDecimal;") {

  } else if (signature == "java.math.BigDecimal.valueOf(Ljava/math/BigInteger;II)Ljava/math/BigDecimal;") {

  } else if (signature == "java.math.BigDecimal.<init>(D)V") {

  }
  return NULL;
#else
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
    sym_bigd->set_ref_exp(handle.get_param_list()[0]);
  } else if (callee_name == "<init>") {
    oop res_obj = handle.get_result<oop>(T_OBJECT);
    SymBigDecimal *sym_bigd = (SymBigDecimal *) ConcolicMngr::ctx->alloc_sym_inst(res_obj);
    sym_bigd->set_ref_exp(new OpSymExpression(handle.get_param_list()[0], op_2d));
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
#endif
}

Expression *SymBigDecimal::get_exp_of(oop obj) {
  if (obj->is_symbolic()) {
    return ConcolicMngr::ctx->get_sym_inst(obj)->get_ref_exp();
  } else {
    return get_con_exp(obj);
  }
}

Expression *SymBigDecimal::get_con_exp(oop obj) {
  oop str_obj = OopUtils::bigd_to_java_string(obj);
  ResourceMark rm;
  const char *str = OopUtils::java_string_to_c(str_obj);
  return new ConSymbolExp(str, T_DOUBLE);
}

void SymBigDecimal::print() {
  tty->print_cr("SymBigDecimal: ");
  _exp->print();
  tty->print("\n");
}

void SymBigDecimal::set_sym_exp(int field_offset, Expression *exp) {
  ShouldNotCallThis();
}

#ifdef ENABLE_WEBRIDGE

void SymBigDecimal::method_exit_callback(MethodSymbolizerHandle &handle) {
}

void SymBigDecimal::set_bigDecimal_symbolic(oop decimalOOp, std::string name) {
  guarantee(decimalOOp != NULL, "Should not be null");
  Klass *bdClz = decimalOOp->klass();
  guarantee(bdClz->name() == vmSymbols::java_math_BigDecimal(), "should equal");
  fieldDescriptor fd_intCmp;
  fieldDescriptor fd_scale;
  bdClz->find_field(
      vmSymbols::bd_intCompact(),
      vmSymbols::long_signature(),
      &fd_intCmp
  );
  bdClz->find_field(
      vmSymbols::bd_scale(),
      vmSymbols::int_signature(),
      &fd_scale
  );

  std::string intCmpName = DECIMAL_WRAPPER
                           + name
                           + "intCompact"
                           + DECIMAL_WRAPPER;

  std::string scaleName = DECIMAL_WRAPPER
                          + name
                          + "scale"
                          + DECIMAL_WRAPPER;

  init_sym_exp(fd_intCmp.offset(), new SymbolExpression(intCmpName.c_str(), intCmpName.length()));
  init_sym_exp(fd_scale.offset(), new SymbolExpression(scaleName.c_str(), scaleName.length()));
}

void SymBigDecimal::init_sym_exp(int field_offset, Expression *exp) {
  guarantee(exp != NULL, "should not be null");
  exp->inc_ref();

  if (_internal_fields.find(field_offset) != _internal_fields.end()) {
    Expression::gc(_internal_fields[field_offset]);
  }
  _internal_fields[field_offset] = exp;
}

Expression *SymBigDecimal::get(int field_offset) {
  return _internal_fields[field_offset];
}

#endif

#endif