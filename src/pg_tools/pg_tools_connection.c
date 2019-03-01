/*-------------------------------------------------------------------------
 *
 * pg_tools_connection.c
 *		Establishes a connection with the database.
 *
 *
 * IDENTIFICATION
 *		pg_tools_connection/pg_tools_connection.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdlib.h>
#include "pg_tools_connection.h"

pg_tools_connection_t pg_tools_open_connection(const char *host,
                                               const char *username,
                                               const char *password) {
    pg_tools_connection_t pg;
    char conninfo[512];

    snprintf(conninfo, sizeof(conninfo),
             "dbname=template1 host=%s user=%s password=%s", host, username,
             password);

    pg.conn = PQconnectdb(conninfo);
    if (PQstatus(pg.conn) == CONNECTION_BAD) {
        fprintf(stderr, "%s\n", PQerrorMessage(pg.conn));
        PQfinish(pg.conn);
        exit(1);
    }

    return pg;
}