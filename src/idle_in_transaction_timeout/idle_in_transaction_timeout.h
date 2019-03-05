#include <libpq-fe.h>

int idle_in_transaction_timeout_kill(PGconn *conn, PGresult *res);