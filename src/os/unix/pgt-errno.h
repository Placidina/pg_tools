#ifndef _PGT_ERRNO_H_INCLUDED_
#define _PGT_ERRNO_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef int pgt_err_t;

#define PGT_EINTR EINTR

#if (__hpux__)
#define PGT_EAGAIN EWOULDBLOCK
#else
#define PGT_EAGAIN EAGAIN
#endif

#define pgt_errno          errno
#define pgt_socket_errno   errno
#define pgt_set_errno(err) errno = err

u_char *pgt_strerror(pgt_err_t err, u_char *errstr, size_t size);
pgt_int_t pgt_strerror_init(void);

#endif /* _PGT_ERRNO_H_INCLUDED */