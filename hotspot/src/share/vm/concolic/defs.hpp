#ifndef SHARE_VM_CONCOLIC_DEFS_HPP
#define SHARE_VM_CONCOLIC_DEFS_HPP

#ifdef ENABLE_CONCOLIC

#include <sys/types.h>

// Symbolioc Object ID related
#define NULL_SYM_OID 0
#define MAX_SYM_OID 10000
typedef ulong sym_oid_t;

#define MAX_SYM_TMP_ID 10000
typedef int sym_tmp_id_t;

#define ARRAY_LENGTH_FIELD_INDEX -1

#endif
#endif // SHARE_VM_CONCOLIC_DEFS_HPP