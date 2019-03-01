/*-------------------------------------------------------------------------
 *
 * pg_tools_config.c
 *		Read configurations from file.
 *
 *
 * IDENTIFICATION
 *		pg_tools_config/pg_tools_config.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdlib.h>
#include "pg_tools_config.h"

static config_t cfg;

void pg_tools_config_read(const char *path, pg_tools_config_t *conf) {
    config_t cfg;
    config_setting_t *setting;

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
}

void pg_tools_config_destroy() { config_destroy(&cfg); }