/*-------------------------------------------------------------------------
 *
 * idle_in_transaction_timeout.c
 *		Kill idle connections of a Postgres server inactive for a given
 *		amount of time.
 *
 *
 * IDENTIFICATION
 *		idle_in_transaction_timeout/idle_in_transaction_timeout.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include "idle_in_transaction_timeout.h"
#include "pg_log.h"

int idle_in_transaction_timeout_kill(PGconn *conn, PGresult *res) {
    res = PQexec(conn, "SELECT pid, datname, application_name, xact_start, "
                       "state_change, query FROM pg_stat_activity WHERE state "
                       "= 'idle in transaction' ORDER BY xact_start LIMIT 1;");

    switch (PQresultStatus(res)) {
    case PGRES_FATAL_ERROR:
        fprintf(stderr, "%s\n", PQerrorMessage(conn));
        PQfinish(conn);
        PQclear(res);
        exit(1);
        break;
    case PGRES_TUPLES_OK: {
        int rows = PQntuples(res);
        if (rows > 0) {
            pg_log(PG_LOG_INFO, "Killed: %s | %s | %s | %s | %s | %s\n",
                   PQgetvalue(res, 0, 0), PQgetvalue(res, 0, 1),
                   PQgetvalue(res, 0, 2), PQgetvalue(res, 0, 3),
                   PQgetvalue(res, 0, 4), PQgetvalue(res, 0, 5));
        }

        PQclear(res);
        return rows;
    }
    default:
        PQclear(res);
        return 0;
    }
}