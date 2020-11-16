#ifndef _PGT_CONFIG_H_INCLUDED_
#define _PGT_CONFIG_H_INCLUDED_

#include <pgt-headers.h>
#include <pgt-linux.h>

#define pgt_cdecl

typedef intptr_t pgt_int_t;
typedef uintptr_t pgt_uint_t;
typedef intptr_t pgt_flag_t;

#define PGT_INT32_LEN (sizeof("-2147483648") - 1)
#define PGT_INT64_LEN (sizeof("-9223372036854775808") - 1)

#if (PGT_PTR_SIZE == 4)
#define PGT_INT_T_LEN       PGT_INT32_LEN
#define PGT_MAX_INT_T_VALUE 2147483647
#else
#define PGT_INT_T_LEN       PGT_INT64_LEN
#define PGT_MAX_INT_T_VALUE 9223372036854775807
#endif

#ifndef pgt_inline
#define pgt_inline inline
#endif

#define PGT_MAX_UINT32_VALUE (uint32_t)0xffffffff

#endif /* _PGT_CONFIG_H_INCLUDED_ */