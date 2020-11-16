#include <pgt-config.h>
#include <pgt-core.h>

static pgt_msec_t pgt_epoch_time(time_t sec, pgt_uint_t msec);

#define PGT_TIME_SLOTS 64

static pgt_uint_t slot;
static pgt_atomic_t pgt_time_lock;

volatile pgt_time_t *pgt_cached_time;
volatile pgt_msec_t pgt_current_msec;
volatile pgt_str_t pgt_cached_err_log_time;

static pgt_int_t cached_gmtoff;
static pgt_time_t cached_time[PGT_TIME_SLOTS];
static u_char cached_err_log_time[PGT_TIME_SLOTS][sizeof("1970/09/28 12:00:00")];

void
pgt_time_init(void)
{
    pgt_cached_err_log_time.len = sizeof("1970/09/28 12:00:00") - 1;
    pgt_cached_time = &cached_time[0];

    pgt_time_update();
}

void
pgt_time_update(void)
{
    u_char *p;
    pgt_tm_t tm, gmt;
    time_t sec;
    pgt_uint_t msec;
    pgt_time_t *tp;
    struct timeval tv;

    if (!pgt_trylock(&pgt_time_lock)) {
        return;
    }

    pgt_gettimeofday(&tv);

    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;

    pgt_current_msec = pgt_epoch_time(sec, msec);

    tp = &cached_time[slot];

    if (tp->sec == sec) {
        tp->msec = msec;
        pgt_unlock(&pgt_time_lock);
        return;
    }

    if (slot == PGT_TIME_SLOTS - 1) {
        slot = 0;
    } else {
        slot++;
    }

    tp = &cached_time[slot];

    tp->sec = sec;
    tp->msec = msec;

    pgt_gmtime(sec, &gmt);

#if (PGT_HAVE_GETTIMEZONE)
    tp->gmtoff = pgt_gettimezone();
    pgt_gmtime(sec + tp->gmtoff * 60, &tm);
#elif (PGT_HAVE_GMTOFF)
    pgt_localtime(sec, &tm);
    cached_gmtoff = (pgt_int_t)(tm.pgt_tm_gmtoff / 60);
    tp->gmtoff = cached_gmtoff;
#else
    pgt_localtime(sec, &tm);
    cached_gmtoff = pgt_timezone(tm.pgt_tm_isdst);
    tp->gmtoff = cached_gmtoff;
#endif

    p = &cached_err_log_time[slot][0];
    (void)pgt_sprintf(p, "%4d/%02d/%02d %02d:%02d:%02d", tm.pgt_tm_year, tm.pgt_tm_mon, tm.pgt_tm_mday, tm.pgt_tm_hour, tm.pgt_tm_min, tm.pgt_tm_sec);

    pgt_memory_barrier();

    pgt_cached_time = tp;
    pgt_cached_err_log_time.data = p;

    pgt_unlock(&pgt_time_lock);
}

static pgt_msec_t
pgt_epoch_time(time_t sec, pgt_uint_t msec)
{
    return (pgt_msec_t)sec * 1000LL + msec;
}

void
pgt_gmtime(time_t t, pgt_tm_t *tp)
{
    pgt_int_t yday;
    pgt_uint_t sec, min, hour, mday, mon, year, wday, days, leap;

    if (t < 0) {
        t = 0;
    }

    days = t / 86400;
    sec = t % 86400;

    if (days > 2932896) {
        days = 2932896;
        sec = 86399;
    }

    wday = (4 + days) % 7;

    hour = sec / 3600;
    sec %= 3600;
    min = sec / 60;
    sec %= 60;

    days = days - (31 + 28) + 719527;
    year = (days + 2) * 400 / (365 * 400 + 100 - 4 + 1);
    yday = days - (365 * year + year / 4 - year / 100 + year / 400);

    if (yday < 0) {
        leap = (year % 4 == 0) && (year % 100 || (year % 400 == 0));
        yday = 365 + leap + yday;
        year--;
    }

    mon = (yday + 31) * 10 / 306;
    mday = yday - (367 * mon / 12 - 30) + 1;

    if (yday >= 306) {
        year++;
        mon -= 10;
    } else {
        mon += 2;
    }

    tp->pgt_tm_sec = (pgt_tm_sec_t)sec;
    tp->pgt_tm_min = (pgt_tm_min_t)min;
    tp->pgt_tm_hour = (pgt_tm_hour_t)hour;
    tp->pgt_tm_mday = (pgt_tm_mday_t)mday;
    tp->pgt_tm_mon = (pgt_tm_mon_t)mon;
    tp->pgt_tm_year = (pgt_tm_year_t)year;
    tp->pgt_tm_wday = (pgt_tm_wday_t)wday;
}