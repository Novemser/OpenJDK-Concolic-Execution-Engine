#ifdef ENABLE_CONCOLIC

#include "concolic/methodSymbolizer.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "concolic/jdbc/reference/symbolicConnection.hpp"
#include "concolic/reference/symbolicString.hpp"
#include "memory/resourceArea.hpp"
#include "runtime/signature.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/ostream.hpp"

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
          .insert(std::make_pair(std::string(class_name), (SymMethodSet *)NULL))
          .first;
  if (sym_class_map_it->second == NULL) {
    sym_class_map_it->second = new SymMethodSet();
  }
  sym_class_map_it->second->insert(std::string(method_name));
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

void MethodSymbolizer::invoke_method(ZeroFrame *caller_frame,
                                     ZeroFrame *callee_frame) {
  if (!caller_frame->is_interpreter_frame()) {
    return;
  }
  if (!callee_frame->is_interpreter_frame()) {
    return;
  }
  // assert(callee_frame->is_interpreter_frame(), "should be");

  _handle.set_caller_frame(caller_frame);
  Method *callee = _handle.get_caller_istate()->callee();
  if (callee == NULL)
    return;

  ResourceMark rm;

  _handle.set_callee_frame(callee_frame);
  _handle.set_callee_holder_name(
      callee->method_holder()->name()->as_C_string());
  _handle.set_callee_name(callee->name()->as_C_string());

  /**
   * Whether we need to symbolize the process of this function
   */
  bool need_symbolize = false;
  SymMethodSet *sym_methods =
      this->get_sym_methods(_handle.get_callee_holder_name());
  
  if (_handle.get_callee_holder_name() == SymString::TYPE_NAME) {
    need_symbolize = SymString::invoke_method(_handle);
  } else if (_handle.get_callee_holder_name() == SymConn::TYPE_NAME) {
    need_symbolize = SymConn::invoke_method(_handle);
  } else if (sym_methods != NULL &&
             sym_methods->find(_handle.get_callee_name()) !=
                 sym_methods->end()) {
    tty->print_cr("Calling function name: %s",
                  callee->name_and_sig_as_C_string());
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
    if (_handle.get_callee_holder_name() == SymString::TYPE_NAME) {
      SymString::finish_method(_handle);
    } else if (_handle.get_callee_holder_name() == SymConn::TYPE_NAME) {
      SymConn::finish_method(_handle);
    } else {
      finish_method_helper(_handle);
    }
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
  int offset = handle.get_begin_offset();
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

void MethodSymbolizer::finish_method_helper(MethodSymbolizerHandle &handle) {
  int offset = handle.get_begin_offset();
  BasicType type = handle.get_result_type();
  Expression *exp;

  if (type == T_OBJECT) {
    ShouldNotReachHere();
  } else if (type == T_ARRAY) {
    ShouldNotReachHere();
  } else {
    exp = new SymbolExpression();
    int delta = type2size[type] - 1;
    assert(delta >= 0, "should be");
    ConcolicMngr::ctx->set_stack_slot(offset + delta, exp);
  }

  ConcolicMngr::record_path_condition(new MethodExpression(
      handle.get_callee_holder_name(), handle.get_callee_name(),
      handle.get_param_list(), exp));
}

int MethodSymbolizer::prepare_param(MethodSymbolizerHandle &handle,
                                    BasicType type, intptr_t *locals,
                                    int offset) {
  Expression *exp;

  if (type == T_OBJECT) {
    oop obj = *(oop *)(locals - offset);
    ConcolicMngr::ctx->get_or_alloc_sym_inst(obj);
    /**
     *  TODO: May be this symbol expression can be used
     */
    exp = new SymbolExpression(obj->get_sym_rid(), SymbolExpression::NULL_INDEX,
                               type);
  } else if (type == T_ARRAY) {
    arrayOop arrObj = *(arrayOop *)(locals - offset);
    SymArr *sym_arr = ConcolicMngr::ctx->get_or_alloc_sym_array(arrObj);
    /**
     *  TODO: May be this symbol expression can be used
     */
    exp = new SymbolExpression(arrObj->get_sym_rid(), sym_arr->get_version(),
                               sym_arr->get_load_count(), type);
  } else {
    offset += type2size[type] - 1;

    exp = ConcolicMngr::ctx->get_stack_slot(offset);
    if (!exp) {
      switch (type) {
      case T_BYTE:
        exp = new ConExpression(*(jbyte *)(locals - offset));
        break;
      case T_CHAR:
        exp = new ConExpression(*(jchar *)(locals - offset));
        break;
      case T_DOUBLE:
        exp = new ConExpression(((VMJavaVal64 *)(locals - offset))->d);
        break;
      case T_FLOAT:
        exp = new ConExpression(*(jfloat *)(locals - offset));
        break;
      case T_INT:
        exp = new ConExpression(*(jint *)(locals - offset));
        break;
      case T_LONG:
        exp = new ConExpression(((VMJavaVal64 *)(locals - offset))->l);
        break;
      case T_SHORT:
        exp = new ConExpression(*(jshort *)(locals - offset));
        break;
      case T_BOOLEAN:
        exp = new ConExpression(*(jboolean *)(locals - offset));
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

#endif