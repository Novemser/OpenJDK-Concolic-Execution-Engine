#ifdef ENABLE_CONCOLIC

#include "concolic/methodSymbolizer.hpp"
#include "concolic/concolicMngr.hpp"
#include "concolic/exp/methodExpression.hpp"
#include "memory/resourceArea.hpp"
#include "runtime/signature.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/ostream.hpp"

void MethodSymbolizer::invoke_method(ZeroFrame *caller_frame,
                                     ZeroFrame *callee_frame) {
  ResourceMark rm;
  if (caller_frame->is_interpreter_frame()) {
    interpreterState caller_istate =
        caller_frame->as_interpreter_frame()->interpreter_state();
    Method *callee = caller_istate->callee();
    if (callee->method_holder()->name()->equals("Example")) {
      tty->print_cr("Calling function name: %s",
                    callee->name_and_sig_as_C_string());
      if (callee->name()->equals("func")) {
        assert(callee_frame->is_interpreter_frame(), "should be");
        interpreterState callee_istate =
            callee_frame->as_interpreter_frame()->interpreter_state();

        int begin_offset =
            caller_istate->stack_base() - callee_istate->locals() - 1;
        int end_offset =
            caller_istate->stack_base() - caller_istate->stack() - 1;
        register intptr_t *locals = ((intptr_t *)callee_istate->locals());

        // Currently, we do not consider "this" object
        begin_offset += 1;

        ResourceMark rm;
        SignatureStream ss(callee->signature());
        while (!ss.at_return_type()) {
          BasicType type = ss.type();
          Expression *exp;

          if (type == T_OBJECT) {
            oop obj = *(oop *)(locals - begin_offset);
            ConcolicMngr::ctx->get_or_alloc_sym_obj(obj);
            /**
             *  TODO: May be this symbol expression can be used
             */
            exp = new SymbolExpression(obj->get_sym_oid(),
                                       SymbolExpression::NULL_INDEX);
          } else if (type == T_ARRAY) {
            arrayOop arrObj = *(arrayOop *)(locals - begin_offset);
            SymArr *sym_arr = ConcolicMngr::ctx->get_or_alloc_sym_array(arrObj);
            /**
             *  TODO: May be this symbol expression can be used
             */
            exp = new SymbolExpression(arrObj->get_sym_oid(),
                                       sym_arr->get_version(),
                                       sym_arr->get_load_count());
          } else {
            exp = ConcolicMngr::ctx->get_stack_slot(begin_offset);
            if (!exp) {
              switch (type) {
              case T_BYTE:
                exp = new ConExpression(*(jbyte *)(locals - begin_offset));
                break;
              case T_CHAR:
                exp = new ConExpression(*(jchar *)(locals - begin_offset));
                break;
              case T_DOUBLE:
                exp = new ConExpression(
                    ((VMJavaVal64 *)(locals - begin_offset - 1))->d);
                break;
              case T_FLOAT:
                exp = new ConExpression(*(jfloat *)(locals - begin_offset));
                break;
              case T_INT:
                exp = new ConExpression(*(jint *)(locals - begin_offset));
                break;
              case T_LONG:
                exp = new ConExpression(
                    ((VMJavaVal64 *)(locals - begin_offset - 1))->l);
                break;
              case T_SHORT:
                exp = new ConExpression(*(jshort *)(locals - begin_offset));
                break;
              case T_BOOLEAN:
                exp = new ConExpression(*(jboolean *)(locals - begin_offset));
                break;
              default:
                ShouldNotReachHere();
                break;
              }
            }
          }

          _param_list.push_back(exp);
          ss.next();
          begin_offset += type2size[type];
        }
        assert(begin_offset == end_offset, "equal");

        // JavaThread *__the_thread__ = ConcolicMngr::ctx->get_thread();
        // oop method_mirror =
        //     Reflection::new_method(callee, true, true, CHECK);
        // objArrayHandle ptypes(
        //     THREAD, objArrayOop(java_lang_reflect_Method::parameter_types(
        //                 method_mirror)));

        // int param_length = callee->method_parameters_length();
        // MethodParametersElement *params = callee->method_parameters_start();
        // methodHandle mh (ConcolicMngr::ctx->get_thread(), callee);

        // tty->print_cr("method_parameters_length: %d", param_length);
        // for (int i = 0; i < param_length; ++i) {
        //   Symbol *sym =
        //       0 != params[i].name_cp_index
        //           ? mh->constants()->symbol_at(params[i].name_cp_index)
        //           : NULL;
        //   if (sym) {
        //     tty->print_cr("param: %s", sym->as_C_string());
        //   }
        // }

        _frame = caller_frame;
        ConcolicMngr::is_symbolizing_method = true;
      }
    }
  }
}

void MethodSymbolizer::finish_method(ZeroFrame *caller_frame) {
  if (caller_frame == _frame) {
    ConcolicMngr::record_path_condition(new MethodExpression(
        "Example", "func", _param_list, new ConExpression(1)));
    this->reset();
    ConcolicMngr::is_symbolizing_method = false;
  }
}

void MethodSymbolizer::reset() {
  _frame = NULL;
  _param_list.clear();
}
#endif