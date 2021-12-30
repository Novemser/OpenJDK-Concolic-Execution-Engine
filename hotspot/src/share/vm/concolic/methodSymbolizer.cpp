#ifdef ENABLE_CONCOLIC

#include "concolic/methodSymbolizer.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/exp/symbolExpression.hpp"
#include "concolic/jdbc/reference/symbolicConnection.hpp"
#include "concolic/jdbc/reference/symbolicResultSet.hpp"
#include "concolic/jdbc/reference/symbolicStatement.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "concolic/reference/symbolicStringBuilder.hpp"
#include "concolic/reference/symbolicMap.hpp"
#include "concolic/reference/symbolicSet.hpp"
#include "concolic/reference/symbolicList.hpp"
#include "concolic/reference/symbolicBigDecimal.hpp"
#include "concolic/reference/symbolicPrimitive.hpp"
#include "concolic/reference/symbolicTimestamp.hpp"
#include "concolic/jdbc/reference/symbolicHibernateKey.hpp"
#include "concolic/jdbc/reference/symbolicHibernateMethod.hpp"
#include "memory/resourceArea.hpp"
#include "runtime/signature.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/ostream.hpp"

MethodSymbolizer::MethodSymbolizer() {
  _symbolizing_method = false;
  init_helper_methods();
}

void MethodSymbolizer::init_helper_methods() {
  SymString::init_register_class(this);
  SymStrBuilder::init_register_class(this);
  SymConn::init_register_class(this);
  SymStmt::init_register_class(this);
  SymResSet::init_register_class(this);
  SymMap::init_register_class(this);
  SymSet::init_register_class(this);
  SymList::init_register_class(this);
  SymBigDecimal::init_register_class(this);
  SymTimestamp::init_register_class(this);
  SymHibernateKey::init_register_class(this);
  SymHibernateMethod::init_register_class(this);
}

void MethodSymbolizer::add_invoke_helper_methods(const std::string class_name,
                                                 bool (*invoke_helper_func)(MethodSymbolizerHandle &)) {
  _invoke_helper_methods[class_name] = invoke_helper_func;
}

void MethodSymbolizer::add_finish_helper_methods(const std::string class_name,
                                                 Expression *(*finish_helper_func)(MethodSymbolizerHandle &)) {
  _finish_helper_methods[class_name] = finish_helper_func;
}

MethodSymbolizer::~MethodSymbolizer() {
  for (SymClassMapIt sym_class_map_it = _symbolicMethods.begin();
       sym_class_map_it != _symbolicMethods.end(); ++sym_class_map_it) {
    if (sym_class_map_it->second) {
      delete sym_class_map_it->second;
    }
  }
}

void MethodSymbolizer::add_method(const char *class_name,
                                  const char *method_name) {
  SymClassMapIt sym_class_map_it =
      _symbolicMethods
          .insert(std::make_pair(std::string(class_name), (SymMethodSet *) NULL))
          .first;
  if (sym_class_map_it->second == NULL) {
    sym_class_map_it->second = new SymMethodSet();
  }
  sym_class_map_it->second->insert(std::string(method_name));
}


void MethodSymbolizer::invoke_method(ZeroFrame *caller_frame,
                                     ZeroFrame *callee_frame) {
  if (!callee_frame->is_interpreter_frame()) {
    return;
  }

  _handle.set_caller_frame(caller_frame);
  _handle.set_callee_frame(callee_frame);
  Method *callee_method = _handle.get_callee_method();
  if (callee_method == NULL)
    return;

  ResourceMark rm;

  _handle.set_callee_holder_name(
      callee_method->method_holder()->name()->as_C_string());
  _handle.set_callee_name(callee_method->name()->as_C_string());

//  if (_handle.get_callee_name().find("prepareStatement") != std::string::npos) {
//    tty->print_cr("%s: %s", _handle.get_callee_holder_name().c_str(),
//                  _handle.get_callee_name().c_str());
//  }

  /**
   * Whether we need to symbolize the process of this function
   */
  bool need_symbolize = false;
  SymMethodSet *sym_methods =
      this->get_sym_methods(_handle.get_callee_holder_name());
  BasicType basicType = primitive_target(_handle.get_callee_holder_name());
  const std::string& callee_holder_name = _handle.get_callee_holder_name();

  if (basicType != T_ILLEGAL) {
    need_symbolize = primitive_invoke_method_helper(_handle, basicType);
  } else if (_invoke_helper_methods.find(callee_holder_name) != _invoke_helper_methods.end()) {
    need_symbolize = _invoke_helper_methods[callee_holder_name](_handle);
  } else if (sym_methods != NULL &&
             sym_methods->find(_handle.get_callee_name()) !=
             sym_methods->end()) {
    tty->print_cr("Calling function name: %s",
                  callee_method->name_and_sig_as_C_string());
    invoke_method_helper(_handle);
    need_symbolize = true;
  }

  if (need_symbolize) {
    this->_symbolizing_method = true;
  } else {
    _handle.reset();
  }
}

void MethodSymbolizer::finish_method(ZeroFrame *caller_frame) {
  if (caller_frame == _handle.get_caller_frame()) {
    Expression *exp = NULL;
    BasicType basicType = primitive_target(_handle.get_callee_holder_name());
    const std::string& callee_holder_name = _handle.get_callee_holder_name();

    if (basicType != T_ILLEGAL) {
      exp = primitive_finish_method_helper(_handle, basicType);
    } else if (_finish_helper_methods.find(callee_holder_name) != _finish_helper_methods.end()) {
      exp = _finish_helper_methods[callee_holder_name](_handle);
    } else {
      exp = finish_method_helper(_handle);
    }

    BasicType type = _handle.get_result_type();
    int delta = type2size[type] > 1; // long and double are 1; 0, otherwise
    assert(delta >= 0, "should be");
    ConcolicMngr::ctx->set_stack_slot_if_possible(
        _handle.get_caller_stack_begin_offset() + delta, exp);
    this->_handle.reset();
    this->_symbolizing_method = false;
  }
}

MethodSymbolizer::SymMethodSet *
MethodSymbolizer::get_sym_methods(const std::string &class_name) {
  SymClassMapIt sym_class_map_it = _symbolicMethods.find(class_name);
  return sym_class_map_it != _symbolicMethods.end() ? sym_class_map_it->second
                                                    : NULL;
}

void MethodSymbolizer::invoke_method_helper(MethodSymbolizerHandle &handle) {
  int offset = handle.get_callee_local_begin_offset();
  register intptr_t *locals = handle.get_locals_ptr();

  // Currently, we do not consider "this" object
  MethodSymbolizer::prepare_param(handle, T_OBJECT, locals, offset);
  ++offset;

  ResourceMark rm;
  SignatureStream ss(handle.get_callee_method()->signature());
  while (!ss.at_return_type()) {
    offset = MethodSymbolizer::prepare_param(handle, ss.type(), locals, offset);

    ss.next();
    ++offset;
  }
  // assert(offset == handle.get_end_offset(), "equal");
}

Expression *
MethodSymbolizer::finish_method_helper(MethodSymbolizerHandle &handle) {
  BasicType type = handle.get_result_type();
  Expression *exp = NULL;
  oop obj = NULL;

  switch (type) {
    case T_VOID:
      exp = SymbolExpression::get(Sym_VOID);
      break;
    case T_OBJECT:
      obj = handle.get_result<oop>(type);
      if (obj != NULL) {
        if (!obj->is_symbolic()) {
          ConcolicMngr::ctx->symbolize(obj);
        }
        /*
          We hope only symbolize the method whose return value
          is the object we support like SymString and SymInterger.
        */
        exp = ConcolicMngr::ctx->get_sym_inst(obj)->get_ref_exp();
        assert(exp != NULL, "should be");
      } else {
        exp = SymbolExpression::get(Sym_NULL);
      }
      break;
    case T_ARRAY:
      ShouldNotCallThis();
      break;
    default:
      exp = new MethodReturnSymbolExp(type);
  }

  ConcolicMngr::record_path_condition(new MethodExpression(
      handle.get_callee_holder_name(), handle.get_callee_name(),
      handle.get_param_list(), exp, true));
  return exp;
}

int MethodSymbolizer::prepare_param(MethodSymbolizerHandle &handle,
                                    BasicType type, intptr_t *locals,
                                    int offset) {
  Expression *exp;

  if (type == T_OBJECT) {
    oop obj = *(oop *) (locals - offset);
    if (obj != NULL) {
      SymInstance *sym_inst = ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
      exp = sym_inst->get_or_create_ref_exp(obj);
    }
  } else if (type == T_ARRAY) {
    arrayOop arrObj = *(arrayOop *) (locals - offset);
    SymArr *sym_arr = ConcolicMngr::ctx->get_or_alloc_sym_array(arrObj);
    /**
     *  TODO: May be this symbol expression can be reused
     */
    exp =
        new ArraySymbolExp(arrObj->get_sym_rid(), sym_arr->get_version(), type);
  } else {
    offset += type2size[type] - 1;

    exp = ConcolicMngr::ctx->get_stack_slot(offset);
    if (!exp) {
      switch (type) {
        case T_BYTE:
          exp = new ConExpression(*(jbyte *) (locals - offset));
          break;
        case T_CHAR:
          exp = new ConExpression(*(jchar *) (locals - offset));
          break;
        case T_DOUBLE:
          exp = new ConExpression(((VMJavaVal64 *) (locals - offset))->d);
          break;
        case T_FLOAT:
          exp = new ConExpression(*(jfloat *) (locals - offset));
          break;
        case T_INT:
          exp = new ConExpression(*(jint *) (locals - offset));
          break;
        case T_LONG:
          exp = new ConExpression(((VMJavaVal64 *) (locals - offset))->l);
          break;
        case T_SHORT:
          exp = new ConExpression(*(jshort *) (locals - offset));
          break;
        case T_BOOLEAN:
          exp = new ConExpression(*(jboolean *) (locals - offset));
          break;
        default:
          ShouldNotReachHere();
          break;
      }
    }
  }

  handle.get_param_list().push_back(exp);
  return offset;
}

void MethodSymbolizer::print() {
  tty->print_cr("symbolic methods:");
  for (SymClassMapIt sym_class_map_it = _symbolicMethods.begin();
       sym_class_map_it != _symbolicMethods.end(); ++sym_class_map_it) {
    tty->print_cr("%s", sym_class_map_it->first.c_str());
    if (sym_class_map_it->second) {
      SymMethodSet *sym_method_set = sym_class_map_it->second;
      for (SymMethodSetIt sym_method_set_it = sym_method_set->begin();
           sym_method_set_it != sym_method_set->end(); ++sym_method_set_it) {
        tty->print_cr("  %s", sym_method_set_it->c_str());
      }
    }
  }
}

sym_rid_t MethodReturnSymbolExp::sym_method_count = 0;

MethodReturnSymbolExp::MethodReturnSymbolExp(BasicType type) {
  stringStream ss(str_buf, BUF_SIZE);
  set_head(ss, 'M', type);
  ss.print("return%lu", sym_method_count++);
  this->finalize(ss.size());
}

#endif