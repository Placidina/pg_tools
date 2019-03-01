/*-------------------------------------------------------------------------
 *
 * pg_tools.c
 *		Tools to management PostgreSQL.
 *
 *
 * IDENTIFICATION
 *		pg_tools/pg_tools.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pg_tools.h"
#include "pg_tools_config.h"
#include "pg_tools_connection.h"
#include "idle_in_transaction_timeout.h"

static void pg_tools_banner(pg_tools_connection_t pg);

int main(int argc, char *argv[]) {
    pg_tools_config_t *conf =
        (pg_tools_config_t *)malloc(sizeof(pg_tools_config_t));
    pg_tools_config_read("resources/pg_tools.conf", conf);

    pg_tools_connection_t pg =
        pg_tools_open_connection(conf->host, conf->username, conf->password);

    pg_tools_banner(pg);

    idle_in_transaction_timeout_t **idles =
        idle_in_transaction_timeout_retrieve(pg.conn, pg.res);

    int len = sizeof(idles) / sizeof(idles[0]);
    printf("Total Idles: %d\n", len);

    for (int i = 0; i < len; i++) {
        printf("%s | %s | %s | %s | %s\n", idles[i]->datname,
               idles[i]->application_name, idles[i]->xact_start,
               idles[i]->state_change, idles[i]->query);
    }

    PQfinish(pg.conn);
    return 0;
}

static void pg_tools_banner(pg_tools_connection_t pg) {
    printf("PG Tools %s\n", STR(version));
    pg.res = PQexec(pg.conn, "SELECT VERSION();");

    if (PQresultStatus(pg.res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "%s\n", PQerrorMessage(pg.conn));
        PQclear(pg.res);
        PQfinish(pg.conn);
        exit(1);
    }

    printf("%s\n", PQgetvalue(pg.res, 0, 0));
    PQclear(pg.res);
}