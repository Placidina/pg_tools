#ifndef _PGT_FILE_H_INCLUDED_
#define _PGT_FILE_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef int pgt_fd_t;

static pgt_inline ssize_t
pgt_write_fd(pgt_fd_t fd, void *buf, size_t n)
{
    return write(fd, buf, n);
}

#define pgt_write_console pgt_write_fd

#define pgt_linefeed(p)   *p++ = LF;
#define PGT_LINEFEED_SIZE 1
#define PGT_LINEFEED      "\x0a"

#define pgt_stdout STDOUT_FILENO
#define pgt_stderr STDERR_FILENO

#endif /* _PGT_FILE_H_INCLUDED_ */