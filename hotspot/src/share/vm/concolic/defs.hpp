#ifndef SHARE_VM_CONCOLIC_DEFS_HPP
#define SHARE_VM_CONCOLIC_DEFS_HPP

#ifdef ENABLE_CONCOLIC

#include "jni.h"

#include <set>
#include <string>
#include <sys/types.h>

// Symbolioc Reference ID related
#define NULL_SYM_RID 0
#define MAX_SYM_RID 100000
typedef ulong sym_rid_t;

#define MAX_SYM_TMP_ID 100000
typedef int sym_tmp_id_t;

typedef ulong tx_id_t;

#define FIELD_INDEX_ARRAY_LENGTH -1

typedef std::set<std::string> method_set_t;

#endif
#endif // SHARE_VM_CONCOLIC_DEFS_HPP