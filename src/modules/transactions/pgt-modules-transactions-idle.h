#ifndef _IDLE_IN_TRANSACTION_TIMEOUT_
#define _IDLE_IN_TRANSACTION_TIMEOUT_

#include <pgt-config.h>
#include <pgt-core.h>

#define MODULES_TRANSACTIONS_IDLE_SESSIN_SKIP "SET SESSION"

typedef struct pgt_modules_transactions_idle_config_s {
    config_t conf;
    int enabled, timeout, session;
} pgt_modules_transactions_idle_config_t;

typedef struct pgt_modules_transactions_idle_s {
    PGconn *conn;
    pgt_modules_transactions_idle_config_t *config;
} pgt_modules_transactions_idle_t;

pgt_modules_transactions_idle_t *pgt_modules_transactions_idle_init(pgt_connection_t *connection, u_char *pgt_conf_file);
pgt_int_t pgt_modules_transactions_idle_timeout_kill(pgt_modules_transactions_idle_t *idle);
void pgt_modules_transactions_idle_destroy(pgt_modules_transactions_idle_t *idle);

#endif /* _IDLE_IN_TRANSACTION_TIMEOUT_ */