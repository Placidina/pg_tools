#ifndef _PGT_ALLOC_H_INCLUDED_
#define _PGT_ALLOC_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

void *pgt_alloc(size_t size);

#define pgt_free free

#endif /* _PGT_ALLOC_H_INCLUDED_ */
