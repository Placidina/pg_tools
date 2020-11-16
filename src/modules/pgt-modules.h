#ifndef _PGT_MODULES_H_INCLUDED_
#define _PGT_MODULES_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef struct pgt_modules_transactions_s {
    pgt_modules_transactions_idle_t *idle;
} pgt_modules_transactions_t;

typedef struct pgt_modules_config_s {
    config_t conf;
    int cycle;
} pgt_modules_config_t;

struct pgt_modules_s {
    pgt_modules_config_t *config;
    pgt_connection_t *connection;
    pgt_modules_transactions_t *transactions;
};

pgt_int_t pgt_modules_init(pgt_connection_t *connection, u_char *pgt_conf_file);
pgt_int_t pgt_modules_cycle();
void pgt_modules_destroy();

#endif /* _PGT_MODULES_H_INCLUDED_ */