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
        fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                config_error_line(&cfg), config_error_text(&cfg));
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "host", &conf->host)) {
        fprintf(stderr, "No 'host' setting in configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "username", &conf->username)) {
        fprintf(stderr, "No 'username' setting in configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "password", &conf->password)) {
        fprintf(stderr, "No 'password' setting in configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_string(&cfg, "log", &conf->log_path)) {
        fprintf(stderr, "No 'log' setting in configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&cfg, "idle_in_transaction_timeout",
                           &conf->idle_in_transaction_timeout)) {
        fprintf(stderr, "No 'idle_in_transaction_timeout' setting in "
                        "configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    if (!config_lookup_int(&cfg, "idle_in_transaction_timeout_daemon",
                           &conf->idle_in_transaction_timeout_daemon)) {
        fprintf(stderr, "No 'idle_in_transaction_timeout_daemon' setting in "
                        "configuration file.\n");
        config_destroy(&cfg);
        exit(EXIT_FAILURE);
    }

    return conf;
}

void pg_config_destroy() { config_destroy(&cfg); }