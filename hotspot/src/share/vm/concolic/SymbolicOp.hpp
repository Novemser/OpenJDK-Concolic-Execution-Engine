#ifndef SHARE_VM_CONCOLIC_OP_HPP
#define SHARE_VM_CONCOLIC_OP_HPP

#ifdef ENABLE_CONCOLIC

enum SymbolicOp {
  op_add = 0,
  op_sub = 1,
  op_mul = 2,
  op_and = 3,
  op_or = 4,
  op_xor = 5,
  op_div = 6,
  op_rem = 7,
  op_lt = 8,
  op_gt = 9,
  op_le = 10,
  op_ge = 11,
  op_eq = 12,
  op_ne = 13,
  op_neg = 14,
  op_cmp = 15,
  op_cmpl = 16,
  op_cmpg = 17,
  op_num,
  op_null
};
static const SymbolicOp NotSymbolicOp[op_num] = {
    op_null, op_null, op_null, op_null, op_null, op_null,
    op_null, op_null, op_ge,   op_le,   op_gt,   op_lt,
    op_ne,   op_eq,   op_null, op_null, op_null, op_null};

static const char *SymbolicOpStr[op_num] = {
    "+", "-",  "*",  "&",  "|",  "^", "/",   "%",    "<",
    ">", "<=", ">=", "==", "!=", "-", "cmp", "cmpl", "cmpg"};

#endif

#endif // SHARE_VM_CONCOLIC_OP_HPP