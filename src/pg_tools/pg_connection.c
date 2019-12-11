/*-------------------------------------------------------------------------
 *
 * pg_connection.c
 *		Establishes a connection with the database.
 *
 *
 * IDENTIFICATION
 *		pg_tools/pg_connection.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdlib.h>
#include "pg_connection.h"

pg_connection_t *pg_open_connection(const char *host, const char *username,
                                    const char *password, const char *name) {
    char conninfo[512];
    pg_connection_t *pg = (pg_connection_t *)malloc(sizeof(pg_connection_t));

    snprintf(conninfo, sizeof(conninfo),
             "dbname=%s host=%s user=%s password=%s", name, host, username,
             password);

    pg->conn = PQconnectdb(conninfo);
    if (PQstatus(pg->conn) == CONNECTION_BAD) {
        fprintf(stderr, "%s\n", PQerrorMessage(pg->conn));
        PQfinish(pg->conn);
        exit(1);
    }

    return pg;
}