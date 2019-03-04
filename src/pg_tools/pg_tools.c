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
#include <signal.h>
#include "pg_tools.h"
#include "pg_config.h"
#include "pg_connection.h"
#include "pg_log.h"
#include "idle_in_transaction_timeout.h"

static pg_connection_t *pg;

static void pg_banner();
static void pg_graceful_shutdown();

int main(int argc, char *argv[]) {
    pg_config_t *conf = pg_config_read("resources/pg_tools.conf");

#ifndef DEBUG
    pg_log_initialize(conf->log_path);
    signal(SIGUSR1, pg_log_reload);
#endif

    pg = pg_open_connection(conf->host, conf->username, conf->password);
    signal(SIGHUP, pg_graceful_shutdown);

    pg_banner();

    idle_in_transaction_timeout_t **idles =
        idle_in_transaction_timeout_retrieve(pg->conn, pg->res);

    int len = sizeof(idles) / sizeof(idle_in_transaction_timeout_t);
    for (int i = 0; i < len; i++) {
        pg_log("%s | %s | %s | %s | %s\n", idles[i]->datname,
               idles[i]->application_name, idles[i]->xact_start,
               idles[i]->state_change, idles[i]->query);
    }

    PQfinish(pg->conn);
    return EXIT_SUCCESS;
}

static void pg_banner() {
    pg_log(PG_LOG_INFO, "PG Tools %s\n", STR(version));
    pg->res = PQexec(pg->conn, "SELECT VERSION();");

    if (PQresultStatus(pg->res) != PGRES_TUPLES_OK) {
        pg_log(PG_LOG_ERROR, "%s\n", PQerrorMessage(pg->conn));
        PQclear(pg->res);
        PQfinish(pg->conn);
        exit(EXIT_FAILURE);
    }

    pg_log(PG_LOG_INFO, "%s\n", PQgetvalue(pg->res, 0, 0));
    PQclear(pg->res);
}

static void pg_graceful_shutdown() {
    PQfinish(pg->conn);
    pg_config_destroy();
    pg_log_destroy();

    exit(EXIT_SUCCESS);
}