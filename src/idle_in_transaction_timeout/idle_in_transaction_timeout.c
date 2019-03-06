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

int idle_in_transaction_timeout_kill(PGconn *conn, PGresult *res, int timeout) {
    char query[QUERY_BUFFER];
    sprintf(query,
            "SELECT pid, datname, application_name, xact_start, state_change, "
            "query FROM pg_stat_activity WHERE (NOW() - state_change) > "
            "INTERVAL '%d seconds' AND state = 'idle in transaction' ORDER BY "
            "xact_start LIMIT 1;",
            timeout);

    res = PQexec(conn, query);
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
            int pid = atoi(PQgetvalue(res, 0, 0));
            pg_log(PG_LOG_INFO, "%d | %s | %s | %s | %s | %s\n", pid,
                   PQgetvalue(res, 0, 1), PQgetvalue(res, 0, 2),
                   PQgetvalue(res, 0, 3), PQgetvalue(res, 0, 4),
                   PQgetvalue(res, 0, 5));

            PQclear(res);
            sprintf(query, "select pg_terminate_backend(%d);", pid);
            printf("%s\n", query);
            res = PQexec(conn, query);

            ExecStatusType status = PQresultStatus(res);
            if (status != PGRES_TUPLES_OK) {
                pg_log(PG_LOG_WARNING, "%s\n", PQresStatus(status));
                pg_log(PG_LOG_ERROR, "%s\n", PQerrorMessage(conn));
            }
        }

        PQclear(res);
        return rows;
    }
    default:
        PQclear(res);
        return 0;
    }
}