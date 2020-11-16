#include <pgt-config.h>
#include <pgt-core.h>

static pgt_str_t err_levels[] = {
    pgt_null_string,
    pgt_string("emerg"),
    pgt_string("alert"),
    pgt_string("crit"),
    pgt_string("error"),
    pgt_string("warn"),
    pgt_string("notice"),
    pgt_string("info"),
    pgt_string("debug")};

void pgt_cdecl
pgt_log_stderr(pgt_err_t err, const char *fmt, ...)
{
    u_char *p, *last;
    va_list args;
    u_char errstr[PGT_MAX_ERROR_STR];

    last = errstr + PGT_MAX_ERROR_STR;

    p = pgt_cpymem(errstr, "pgtools: ", 9);

    va_start(args, fmt);
    p = pgt_vslprintf(p, last, fmt, args);
    va_end(args);

    if (err) {
        p = pgt_log_errno(p, last, err);
    }

    if (p > last - PGT_LINEFEED_SIZE) {
        p = last - PGT_LINEFEED_SIZE;
    }

    pgt_linefeed(p);

    (void)pgt_write_console(pgt_stderr, errstr, p - errstr);
}

u_char *
pgt_log_errno(u_char *buf, u_char *last, pgt_err_t err)
{
    if (buf > last - 50) {
        buf = last - 50;
        *buf++ = '.';
        *buf++ = '.';
        *buf++ = '.';
    }

    buf = pgt_slprintf(buf, last, " (%d: ", err);
    buf = pgt_strerror(err, buf, last - buf);

    if (buf < last) {
        *buf++ = ')';
    }

    return buf;
}

void
pgt_log_error_core(pgt_uint_t level, pgt_err_t err, const char *fmt, ...)
{
    va_list args;
    u_char *p, *last;
    u_char errstr[PGT_MAX_ERROR_STR];

    last = errstr + PGT_MAX_ERROR_STR;

    p = pgt_cpymem(errstr, pgt_cached_err_log_time.data, pgt_cached_err_log_time.len);
    p = pgt_slprintf(p, last, " [%V] ", &err_levels[level]);

    va_start(args, fmt);
    p = pgt_vslprintf(p, last, fmt, args);
    va_end(args);

    if (err) {
        p = pgt_log_errno(p, last, err);
    }

    if (p > last - PGT_LINEFEED_SIZE) {
        p = last - PGT_LINEFEED_SIZE;
    }

    pgt_linefeed(p);
    pgt_write_fd(pgt_stderr, errstr, p - errstr);
}