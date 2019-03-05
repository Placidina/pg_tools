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
static void pg_args_help(FILE *fp, int exit_code);

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "c:h")) != -1) {
        switch (opt) {
        case 'c':
            conf_path = optarg;
            break;
        case 'h':
            pg_args_help(stdout, EXIT_FAILURE);
            break;
        default:
            pg_args_help(stderr, EXIT_FAILURE);
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

    while (1) {
        if (!idle_in_transaction_timeout_kill(pg->conn, pg->res)) {
            sleep(1);
        }
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

static void pg_args_help(FILE *fp, int exit_code) {
    char arg_help[32], arg_config[32];

    sprintf(arg_help, "-h\tShow this.");
    sprintf(arg_config, "-c\tConfiguration file.\n\t\t\tDefault: %s",
            conf_path);

    fprintf(fp,
            "PG Tools %s\n\nUsage:\n\t%s\n\t%s\n\nReport bug: "
            "<alanplacidina@gmail.com>\n",
            STR(version), arg_help, arg_config);
    exit(exit_code);
}