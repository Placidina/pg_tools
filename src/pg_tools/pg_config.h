#ifndef _PG_CONFIG
#define _PG_CONFIG

#include <libconfig.h>

typedef struct pg_config_t {
    const char *log_path;
    const char *database_host;
    const char *database_username;
    const char *database_password;
#ifndef LIBCONFIG_VER_MAJOR
    long int idle_in_transaction_timeout;
    long int idle_in_transaction_daemon;
#else
    int idle_in_transaction_timeout;
    int idle_in_transaction_daemon;
#endif
} pg_config_t;

/*
 * Read configuration file
 */
pg_config_t *pg_config_read(const char *path);

/*
 * Destroy 'config_t' in memory
 */
void pg_config_destroy();
#endif