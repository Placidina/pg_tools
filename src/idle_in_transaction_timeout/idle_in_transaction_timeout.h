#include <libpq-fe.h>

typedef struct idle_in_transaction_timeout_t {
    char *datname;
    char *application_name;
    char *xact_start;
    char *state_change;
    char *query;
} idle_in_transaction_timeout_t;

idle_in_transaction_timeout_t **
idle_in_transaction_timeout_retrieve(PGconn *conn, PGresult *res);