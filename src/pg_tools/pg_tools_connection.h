#include <libpq-fe.h>

typedef struct pg_tools_connection_t {
    PGconn *conn;
    PGresult *res;
} pg_tools_connection_t;

/*
 * Open a connection with the database server
 */
pg_tools_connection_t pg_tools_open_connection(const char *host,
                                               const char *username,
                                               const char *password);