#ifndef _PGT_ATOMIC_H_INCLUDED_
#define _PGT_ATOMIC_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef long pgt_atomic_int_t;
typedef unsigned long pgt_atomic_uint_t;

#if (PGT_PTR_SIZE == 8)
#define PGT_ATOMIC_T_LEN (sizeof("-9223372036854775808") - 1)
#else
#define PGT_ATOMIC_T_LEN (sizeof("-2147483648") - 1)
#endif

typedef volatile pgt_atomic_uint_t pgt_atomic_t;

#define pgt_atomic_cmp_set(lock, old, set) \
    __sync_bool_compare_and_swap(lock, old, set)

#define pgt_memory_barrier() __sync_synchronize()

#define pgt_trylock(lock) (*(lock) == 0 && pgt_atomic_cmp_set(lock, 0, 1))
#define pgt_unlock(lock)  *(lock) = 0

#endif /* _PGT_ATOMIC_H_INCLUDED_ */