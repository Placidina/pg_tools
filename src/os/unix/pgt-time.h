#ifndef _PGT_TIME_H_INCLUDED_
#define _PGT_TIME_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef pgt_uint_t pgt_msec_t;
typedef pgt_int_t pgt_msec_int_t;

typedef struct tm pgt_tm_t;

#define pgt_tm_sec   tm_sec
#define pgt_tm_min   tm_min
#define pgt_tm_hour  tm_hour
#define pgt_tm_mday  tm_mday
#define pgt_tm_mon   tm_mon
#define pgt_tm_year  tm_year
#define pgt_tm_wday  tm_wday
#define pgt_tm_isdst tm_isdst

#define pgt_tm_sec_t  int
#define pgt_tm_min_t  int
#define pgt_tm_hour_t int
#define pgt_tm_mday_t int
#define pgt_tm_mon_t  int
#define pgt_tm_year_t int
#define pgt_tm_wday_t int

#if (PGT_HAVE_GMTOFF)
#define pgt_tm_gmtoff tm_gmtoff
#endif

void pgt_localtime(time_t s, pgt_tm_t *tm);

#define pgt_gettimeofday(tp) (void)gettimeofday(tp, NULL);

#endif /* _PGT_TIME_H_INCLUDED_ */