#include <pgt-config.h>
#include <pgt-core.h>

void
pgt_localtime(time_t s, pgt_tm_t *tm)
{
    (void)localtime_r(&s, tm);

    tm->pgt_tm_mon++;
    tm->pgt_tm_year += 1900;
}