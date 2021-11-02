#ifndef SHARE_VM_CONCOLIC_OP_HPP
#define SHARE_VM_CONCOLIC_OP_HPP

#ifdef ENABLE_CONCOLIC

enum SymbolicOp {
  op_add = 0,
  op_sub,
  op_mul,
  op_and,
  op_or,
  op_xor,
  op_div,
  op_rem,
  op_lt,
  op_gt,
  op_le,
  op_ge,
  op_eq,
  op_ne,
  op_num,
  op_null
};

static const char *SymbolicOpStr[op_num] = {
    "+", "-", "*", "&", "|", "^", "/", "%", "<", ">", "<=", ">=", "==", "!="};

#endif

#endif // SHARE_VM_CONCOLIC_OP_HPP