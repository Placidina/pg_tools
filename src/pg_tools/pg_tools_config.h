#include <libconfig.h>

#define CONFIG_BUFFER_SIZE 1024

typedef struct pg_tools_config_t {
    const char *host;
    const char *username;
    const char *password;
} pg_tools_config_t;

/*
 * Read configuration file
 */
void pg_tools_config_read(const char *path, pg_tools_config_t *conf);

/*
 * Destroy 'config_t' in memory
 */
void pg_tools_config_destroy();