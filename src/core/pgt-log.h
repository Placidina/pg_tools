#ifndef _PGT_LOG_H_INCLUDED_
#define _PGT_LOG_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

#define PGT_LOG_STDERR 0
#define PGT_LOG_EMERG  1
#define PGT_LOG_ALERT  2
#define PGT_LOG_CRIT   3
#define PGT_LOG_ERR    4
#define PGT_LOG_WARN   5
#define PGT_LOG_NOTICE 6
#define PGT_LOG_INFO   7
#define PGT_LOG_DEBUG  8

#define PGT_MAX_ERROR_STR 2048

#define pgt_log_error(level, ...) \
    pgt_log_error_core(level, __VA_ARGS__)
#define pgt_log_debug(...) \
    if (PGT_DEBUG)         \
    pgt_log_error_core(PGT_LOG_DEBUG, 0, __VA_ARGS__)
#define pgt_log_debug_error(err, ...) \
    if (PGT_DEBUG)                    \
    pgt_log_error_core(PGT_LOG_DEBUG, err, __VA_ARGS__)

void pgt_cdecl pgt_log_stderr(pgt_err_t err, const char *fmt, ...);
u_char *pgt_log_errno(u_char *buf, u_char *last, pgt_err_t err);
void pgt_log_error_core(pgt_uint_t level, pgt_err_t err, const char *fmt, ...);

static pgt_inline void
pgt_write_stderr(char *text)
{
    (void)pgt_write_fd(pgt_stderr, text, pgt_strlen(text));
}

static pgt_inline void
pgt_write_stdout(char *text)
{
    (void)pgt_write_fd(pgt_stdout, text, pgt_strlen(text));
}

#endif /* _PGT_LOG_H_INCLUDED_ */