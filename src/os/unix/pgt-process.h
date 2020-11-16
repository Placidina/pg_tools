#ifndef _PGT_PROCESS_H_INCLUDED_
#define _PGT_PROCESS_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

#define pgt_getpid getpid

typedef pid_t pgt_pid_t;

pgt_int_t pgt_process_init();

extern pgt_pid_t pgt_pid;
extern pgt_int_t pgt_max_sockets;
extern pgt_flag_t pgt_signal_terminate;

#endif /* _PGT_PROCESS_H_INCLUDED_ */