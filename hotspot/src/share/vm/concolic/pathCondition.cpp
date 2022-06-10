#ifdef ENABLE_CONCOLIC

#include "concolic/concolicMngr.hpp"
#include "concolic/pathCondition.hpp"
#include "utilities/ostream.hpp"
#include "webridge/utils/rapidjson/document.h"

void __attribute__((optimize("O0"))) PathCondition::add(Expression *exp) {
  const std::string &code_pos = ConcolicMngr::ctx->get_current_code_pos();
//  guarantee(code_pos.find("valueOf") == std::string::npos, "we should permit cache when use valueOf");
  Condition *cond = new Condition(code_pos, exp);
  _conds.push_back(cond);
}

void PathCondition::gc() {
  int size = _conds.size();
  for (int i = 0; i < size; ++i) {
    delete _conds[i];
  }
  _conds.clear();
  if (_pc_str != NULL) {
    delete _pc_str;
    _pc_str = NULL;
  }
}

void PathCondition::print() {
  tty->print_cr("PathCondition:");
  int size = _conds.size();
  for (int i = 0; i < size; ++i) {
    tty->print("[exp:%d]\t", i);
    Condition *cond = _conds[i];
    tty->print_cr("%s", cond->_code_pos.c_str());
    tty->print("\t\t  ");
    cond->_exp->print_cr();
  }
}

#endif
