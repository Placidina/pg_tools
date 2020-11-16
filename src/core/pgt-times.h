#ifndef _PGT_TIMES_H_INCLUDED_
#define _PGT_TIMES_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef struct {
    time_t sec;
    pgt_uint_t msec;
    pgt_int_t gmtoff;
} pgt_time_t;

void pgt_time_init(void);
void pgt_time_update(void);
void pgt_gmtime(time_t t, pgt_tm_t* tp);

extern volatile pgt_time_t* pgt_cached_time;
extern volatile pgt_msec_t pgt_current_msec;
extern volatile pgt_str_t pgt_cached_err_log_time;

#define pgt_timeofday() (pgt_time_t*)pgt_cached_time

#endif /* _PGT_TIME_H_INCLUDED_ */