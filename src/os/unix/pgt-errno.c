#include <pgt-config.h>
#include <pgt-core.h>

static pgt_str_t *pgt_sys_errlist;
static pgt_str_t pgt_unknown_error = pgt_string("Unknown error");

u_char *
pgt_strerror(pgt_err_t err, u_char *errstr, size_t size)
{
    pgt_str_t *msg;

    msg = ((pgt_uint_t)err < PGT_SYS_NERR) ? &pgt_sys_errlist[err] : &pgt_unknown_error;
    size = pgt_min(size, msg->len);

    return pgt_cpymem(errstr, msg->data, size);
}

pgt_int_t
pgt_strerror_init(void)
{
    char *msg;
    u_char *p;
    size_t len;
    pgt_err_t err;

    len = PGT_SYS_NERR * sizeof(pgt_str_t);

    pgt_sys_errlist = malloc(len);
    if (pgt_sys_errlist == NULL) {
        goto failed;
    }

    for (err = 0; err < PGT_SYS_NERR; err++) {
        msg = strerror(err);
        len = pgt_strlen(msg);

        p = malloc(len);
        if (p == NULL) {
            goto failed;
        }

        pgt_memcpy(p, msg, len);
        pgt_sys_errlist[err].len = len;
        pgt_sys_errlist[err].data = p;
    }

    return PGT_OK;

failed:

    err = errno;
    pgt_log_stderr(0, "malloc(%uz) failed (%d: %s)", len, err, strerror(err));

    return PGT_ERROR;
}
