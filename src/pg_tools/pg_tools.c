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
#include <unistd.h>
#include <getopt.h>
#include "pg_tools.h"
#include "pg_config.h"
#include "pg_connection.h"
#include "pg_log.h"
#include "idle_in_transaction_timeout.h"

static pg_connection_t *pg;
static const char *conf_path = "/etc/pg_tools/pg_tools.conf";

static void pg_banner();
static void pg_graceful_shutdown();

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
        case 'c':
            conf_path = optarg;
            break;
        default:
            fprintf(stderr, "Usage:\n\t-c\tConfiguration file.\n\n");
            exit(EXIT_FAILURE);
        }
    }

    pg_config_t *conf = pg_config_read(conf_path);

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
        pg_log(PG_LOG_INFO, "%s | %s | %s | %s | %s\n", idles[i]->datname,
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
#ifndef DEBUG
    pg_log_destroy();
#endif

    exit(EXIT_SUCCESS);
}