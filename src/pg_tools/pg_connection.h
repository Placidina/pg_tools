#include <libpq-fe.h>

typedef struct pg_connection_t {
    PGconn *conn;
    PGresult *res;
} pg_connection_t;

/*
 * Open a connection with the database server
 */
pg_connection_t *pg_open_connection(const char *host, const char *username,
                                    const char *password);