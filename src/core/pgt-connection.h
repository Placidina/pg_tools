#ifndef _PGT_CONNECTION_H_INCLUDED_
#define _PGT_CONNECTION_H_INCLUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef struct pgt_connection_config_database_s {
    const char *host;
    const char *username;
    const char *password;
    const char *name;
} pgt_connection_config_database_t;

typedef struct pgt_connection_config_s {
    config_t conf;
    pgt_connection_config_database_t *database;
} pgt_connection_config_t;

struct pgt_connection_s {
    PGconn *conn;
    pgt_connection_config_t *config;
};

pgt_connection_t *pgt_connection_init(u_char *pgt_conf_file);
void pgt_connection_destroy(pgt_connection_t *connection);

#endif /* _PGT_CONNECTION_H_INCLUDED_ */