/*-------------------------------------------------------------------------
 *
 * pg_config.c
 *		Read configurations from file.
 *
 *
 * IDENTIFICATION
 *		pg_tools/pg_config.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>
#include "pg_config.h"

static config_t cfg;

pg_config_t *pg_config_read(const char *path) {
    config_t cfg;
    config_setting_t *setting;
    pg_config_t *conf = (pg_config_t *)malloc(sizeof(pg_config_t));

    config_init(&cfg);

    if (!config_read_file(&cfg, path)) {
        fprintf(stderr, "%s\n", config_error_text(&cfg));
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "log", &conf->log_path)) {
        fprintf(stderr,
                "Unable to retrieve log path from configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "database.host", &conf->database_host)) {
        fprintf(stderr,
                "Unable to retrieve database host from configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "database.username",
                              &conf->database_username)) {
        fprintf(
            stderr,
            "Unable to retrieve database username from configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "database.password",
                              &conf->database_password)) {
        fprintf(
            stderr,
            "Unable to retrieve database password from configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "database.name", &conf->database_name)) {
        fprintf(stderr,
                "Unable to retrieve database name from configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&cfg, "idle_in_transaction.timeout",
                           &conf->idle_in_transaction_timeout)) {
        fprintf(stderr, "Unable to retrieve idle in transaction timeout from "
                        "configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&cfg, "idle_in_transaction.daemon",
                           &conf->idle_in_transaction_daemon)) {
        fprintf(stderr, "Unable to retrieve idle in transaction daemon from "
                        "configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    return conf;
}

void pg_config_destroy() { config_destroy(&cfg); }