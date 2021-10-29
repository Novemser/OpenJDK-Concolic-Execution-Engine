#ifndef SHARE_VM_CONCOLIC_SYMBOLICEXPRESSION_HPP
#define SHARE_VM_CONCOLIC_SYMBOLICEXPRESSION_HPP

#ifdef ENABLE_CONCOLIC

#include "utilities/ostream.hpp"

#include <stdio.h>

class SymbolicExpression {
  static const int EXP_NAME_LENGTH = 16;

private:
  union {
    char exp[EXP_NAME_LENGTH];
    struct {
      SymbolicExpression *left;
      SymbolicExpression *right;
    };
  } _data;
  uint _ref_count;
  char _op;
  bool _is_leaf;

public:
  SymbolicExpression(char *sym_name, int field_index)
      : _ref_count(0), _op(0), _is_leaf(true) {
    int ret = sprintf(_data.exp, "%s.%d", sym_name, field_index);
    assert(ret > 0, "SYM_NAME_LENGTH exceeded!");
  }

  SymbolicExpression(SymbolicExpression *l, SymbolicExpression *r, char op)
      : _ref_count(0), _op(op), _is_leaf(false) {
    _data.left = l;
    _data.right = r;
    
    l->inc_ref();
    r->inc_ref();
  }

  void inc_ref() { _ref_count += 1; }

  void print() {
    if (_is_leaf) {
      tty->print("%s\n", _data.exp);
    } else {
      _data.left->print();
      tty->print("%c\n", _op);
      _data.right->print();
    }
  }
};

#endif

#endif // SHARE_VM_CONCOLIC_SYMBOLICEXPRESSION_HPP