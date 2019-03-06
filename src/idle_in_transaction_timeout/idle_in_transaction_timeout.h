#ifndef _IDLE_IN_TRANSACTION_TIMEOUT
#define _IDLE_IN_TRANSACTION_TIMEOUT
#define QUERY_BUFFER 512

#include <libpq-fe.h>

int idle_in_transaction_timeout_kill(PGconn *conn, PGresult *res, int timeout);
#endif