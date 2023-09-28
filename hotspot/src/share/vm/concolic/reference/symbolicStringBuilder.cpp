#ifdef ENABLE_CONCOLIC

#include <concolic/exp/stringExpression.hpp>
#include "concolic/reference/symbolicStringBuilder.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/arrayInitExpression.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/utils.hpp"

const char *SymStrBuilder::TYPE_NAME = "java/lang/StringBuilder";
method_set_t SymStrBuilder::handle_method_names = init_handle_method_names();
std::map<std::string, bool> SymStrBuilder::skip_method_names =
    init_skip_method_names();

method_set_t SymStrBuilder::init_handle_method_names() {
  method_set_t m_set;
  m_set.insert("append");
  m_set.insert("toString");
  m_set.insert("length");
  return m_set;
}

std::map<std::string, bool> SymStrBuilder::init_skip_method_names() {
  std::map<std::string, bool> map;
  map["<init>"] = true;
  map["setLength"] = true;
  return map;
}

void SymStrBuilder::init_register_class(MethodSymbolizer *m_symbolizer) {
  m_symbolizer->add_invoke_helper_methods(SymStrBuilder::TYPE_NAME, invoke_method_helper);
  m_symbolizer->add_finish_helper_methods(SymStrBuilder::TYPE_NAME, finish_method_helper);
}

SymStrBuilder::SymStrBuilder(sym_rid_t sym_rid)
    : SymInstance(sym_rid), _exp(NULL), _ref_exp(NULL) {
}

SymStrBuilder::~SymStrBuilder() {
  Expression::gc(_exp);
  Expression::gc(_ref_exp);
}

void SymStrBuilder::print() {
  tty->print_cr("SymStrBuilder: ");
  Expression::print_on_maybe_null(_ref_exp);
}

bool SymStrBuilder::invoke_method_helper(MethodSymbolizerHandle &handle) {
  const std::string &callee_name = handle.get_callee_name();
  bool need_symbolize = false;
  ResourceMark mr;
  if (handle_method_names.find(callee_name) != handle_method_names.end()) {
    need_symbolize = handle.general_check_param_symbolized();
    if (need_symbolize) {
      SymStrBuilder::prepare_param(handle);
    }
  } else {
    std::map<std::string, bool>::iterator iter =
        skip_method_names.find(callee_name);
    if (iter != skip_method_names.end()) {
      need_symbolize = iter->second;
      if (!need_symbolize) {
        bool recording = handle.general_check_param_symbolized();
        handle.get_callee_method()->print_name(tty);
//        tty->print_cr(" skipped by SymStrBuilder, need recording %c",
//                      recording ? 'Y' : 'N');
      }
    } else {
      bool recording = handle.general_check_param_symbolized();
      handle.get_callee_method()->print_name(tty);
//      tty->print_cr(" handled by SymStrBuilder, need recording %c",
//                    recording ? 'Y' : 'N');
    }
  }
  return need_symbolize;
}

void SymStrBuilder::prepare_param(MethodSymbolizerHandle &handle) {
  Method *callee_method = handle.get_callee_method();

  int offset = handle.get_callee_local_begin_offset();

  if (!callee_method->is_static()) {
    SymStrBuilder::prepare_param_helper(handle, T_OBJECT, offset);
    ++offset;
  }

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
  while (!ss.at_return_type()) {
    offset = SymStrBuilder::prepare_param_helper(handle, ss.type(), offset);
    ss.next();
    ++offset;
  }
}

int SymStrBuilder::prepare_param_helper(MethodSymbolizerHandle &handle,
                                        BasicType type, int locals_offset) {
  Expression *exp = NULL;

  if (is_java_primitive(type)) {
    exp = OpStrExpression::to_string(handle.get_primitive_exp(locals_offset, type));
    locals_offset += type2size[type] - 1;
  } else if (type == T_OBJECT) {
    oop obj = handle.get_param<oop>(locals_offset);
    if (obj == NULL) {
      if (handle.get_callee_name() == "append") {
        exp = new ConStringSymbolExp(std::string("null"));
      } else {
        ShouldNotCallThis();
      }
    } else {
      exp = SymStrBuilder::get_exp_of(obj);
    }

  } else if (type == T_ARRAY) {
    tty->print_cr("record string method having a array param: ");
    handle.get_callee_method()->print_name(tty);
    tty->cr();

    arrayOop arr_obj = handle.get_param<arrayOop>(locals_offset);
    if (arr_obj->is_symbolic()) {
      SymArr *sym_arr = ConcolicMngr::ctx->get_sym_array(arr_obj);
      exp = new ArraySymbolExp(arr_obj->get_sym_rid(), sym_arr->get_version(),
                               type);
    } else {
      exp = new ArrayInitExpression(NULL_SYM_RID, arr_obj);
    }
  } else {
    tty->print_cr("unhandled String parameter types!");
    ShouldNotCallThis();
  }

  handle.get_param_list().push_back(exp);
  return locals_offset;
}

void __attribute__((optimize("O0"))) SymStrBuilder::add_to_string_for_param_list(MethodSymbolizerHandle &handle, exp_list_t &ret) {
  exp_list_t &param_list = handle.get_param_list();

  Method *callee_method = handle.get_callee_method();

  int offset = handle.get_callee_local_begin_offset();

  ResourceMark rm;
  SignatureStream ss(callee_method->signature());
//  callee_method->signature()->print();
//  tty->cr();
  // skip `this`
  offset += 1;
  int i = 1;
  ret.push_back(param_list[0]);
  while (!ss.at_return_type()) {
    BasicType type = ss.type();
    offset = SymString::prepare_param_helper(handle, ss.type(), offset);
    oop obj = handle.get_param<oop>(offset);
    if (obj == NULL && param_list[i] != NULL) {
      ret.push_back(param_list[i]);
    } else if (type == T_OBJECT && obj->is_instance() &&
        (obj->klass()->name()->equals(SymString::TYPE_NAME) || obj->klass()->name()->equals(SymStrBuilder::TYPE_NAME))
        || (param_list[i]->is_op_str_expression() &&
            strncmp("toString", ((OpStrExpression *) param_list[i])->get_op_name().c_str(), 8) == 0)) {
      ret.push_back(param_list[i]);
    } else {
      ret.push_back(new OpStrExpression("toString", param_list[i]));
    }
    i++;
    ss.next();
    offset += type2size[type];
  }
}

Expression *
SymStrBuilder::finish_method_helper(MethodSymbolizerHandle &handle) {
  BasicType type = handle.get_result_type();
  Expression *exp = NULL;
  oop obj = handle.get_result<oop>(type);

  const std::string &callee_name = handle.get_callee_name();
  if (callee_name == "append") {
    SymStrBuilder *sym_res_strBuilder =
        (SymStrBuilder *)ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
    exp_list_t string_exp_list;
    add_to_string_for_param_list(handle, string_exp_list);
    sym_res_strBuilder->set_ref_exp(
        new OpStrExpression("concat", string_exp_list));
  } else if (callee_name == "toString") {
    SymString *sym_res_str =
        (SymString *)ConcolicMngr::ctx->alloc_sym_inst(obj);
    oop obj = handle.get_param<oop>(0);
    SymInstance *sym_inst = ConcolicMngr::ctx->get_sym_inst(obj);
    sym_res_str->set_ref_exp(sym_inst->get_ref_exp());
  }
  return exp;
}

Expression *SymStrBuilder::get_exp_of(oop obj) {
  if (obj->is_symbolic()) {
    return ConcolicMngr::ctx->get_sym_inst(obj)->get_ref_exp();
  } else {
    return new ConStringSymbolExp(obj);
  }
}

#endif