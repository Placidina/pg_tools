#ifndef _PGT_HEADERS_H_INCLUDED_
#define _PGT_HEADERS_H_INCLUDED_

#include <pgt-config.h>

typedef struct pgt_connection_s pgt_connection_t;
typedef struct pgt_modules_s pgt_modules_t;

#define PGT_OK    0
#define PGT_ERROR -1
#define PGT_AGAIN -2
#define PGT_ABORT -3

#include <pgt-errno.h>
#include <pgt-atomic.h>
#include <pgt-time.h>
#include <pgt-string.h>
#include <pgt-files.h>
#include <pgt-process.h>
#include <pgt-log.h>
#include <pgt-alloc.h>
#include <pgt-times.h>
#include <pgt-connection.h>

#include <pgt-modules-transactions-idle.h>
#include <pgt-modules.h>

#define LF (u_char)'\n'
#define CR (u_char)'\r'

#define pgt_max(val1, val2) ((val1 < val2) ? (val2) : (val1))
#define pgt_min(val1, val2) ((val1 > val2) ? (val2) : (val1))

#endif /* _PGT_HEADERS_H_INCLUDED_ */