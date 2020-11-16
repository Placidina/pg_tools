#include <pgt-config.h>
#include <pgt-core.h>

void *
pgt_alloc(size_t size)
{
    void *p;

    p = malloc(size);
    if (p == NULL) {
        pgt_log_error(PGT_LOG_EMERG, pgt_errno, "malloc(%uz) failed", size);
    }

    return p;
}