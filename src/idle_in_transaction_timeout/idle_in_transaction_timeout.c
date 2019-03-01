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

idle_in_transaction_timeout_t **
idle_in_transaction_timeout_retrieve(PGconn *conn, PGresult *res) {
    res = PQexec(conn, "SELECT datname, application_name, xact_start, "
                       "state_change, query FROM pg_stat_activity WHERE state "
                       "= 'idle in transaction' ORDER BY xact_start;");

    if (PQresultStatus(res) == PGRES_FATAL_ERROR) {
        fprintf(stderr, "%s\n", PQerrorMessage(conn));
        PQfinish(conn);
        PQclear(res);
        exit(1);
    }

    int rows = PQntuples(res);
    int fields = PQnfields(res);

    idle_in_transaction_timeout_t **idle =
        (idle_in_transaction_timeout_t **)malloc(
            sizeof(idle_in_transaction_timeout_t) * rows);

    for (int r = 0; r < rows; r++) {
        char *values[fields];
        for (int f = 0; f < fields; f++) {
            values[f] = PQgetvalue(res, r, f);
        }

        idle[r] = (idle_in_transaction_timeout_t *)malloc(
            sizeof(idle_in_transaction_timeout_t));

        idle[r]->datname = values[0];
        idle[r]->application_name = values[1];
        idle[r]->xact_start = values[2];
        idle[r]->state_change = values[3];
        idle[r]->query = values[4];
    }

    PQclear(res);
    return idle;
}