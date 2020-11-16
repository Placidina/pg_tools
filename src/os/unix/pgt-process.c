#include <pgt-config.h>
#include <pgt-core.h>

static void pgt_signal_handler(int signo);

pgt_pid_t pgt_pid;
pgt_int_t pgt_max_sockets;
struct rlimit rlmt;

pgt_flag_t pgt_signal_terminate;

pgt_int_t
pgt_process_init()
{
    pgt_time_t *tp;

    pgt_pid = pgt_getpid();

    if (getrlimit(RLIMIT_NOFILE, &rlmt) == -1) {
        pgt_log_error(PGT_LOG_ALERT, errno, "getrlimit(RLIMIT_NOFILE) failed");
        return PGT_ERROR;
    }

    pgt_max_sockets = (pgt_int_t)rlmt.rlim_cur;

    tp = pgt_timeofday();
    srandom(((unsigned)pgt_pid << 16) ^ tp->sec ^ tp->msec);

    signal(SIGQUIT, pgt_signal_handler);
    signal(SIGINT, pgt_signal_handler);

    return PGT_OK;
}

static void
pgt_signal_handler(int signo)
{
    pgt_err_t err = pgt_errno;

    switch (signo) {
        case SIGQUIT:
        case SIGINT:
            pgt_signal_terminate = 1;
            break;
    }

    pgt_set_errno(err);
}