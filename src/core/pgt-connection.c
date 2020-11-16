#include <pgt-config.h>
#include <pgt-core.h>

static pgt_connection_config_t *pgt_connection_config_init(u_char *pgt_conf_file);

pgt_connection_t *
pgt_connection_init(u_char *pgt_conf_file)
{
    u_char conninfo[1024];
    pgt_connection_t *pg;

    pg = pgt_alloc(sizeof(pgt_connection_t));
    if (pg == NULL) {
        return NULL;
    }

    pg->config = pgt_connection_config_init(pgt_conf_file);
    if (pg->config == NULL) {
        return NULL;
    }

    pgt_sprintf(conninfo, "dbname=%s host=%s user=%s password=%s",
                pg->config->database->name, pg->config->database->host, pg->config->database->username, pg->config->database->password);

    pg->conn = PQconnectdb((const char *)conninfo);
    if (PQstatus(pg->conn) == CONNECTION_BAD) {
        pgt_log_error(PGT_LOG_EMERG, 0, "%s", PQerrorMessage(pg->conn));
        PQfinish(pg->conn);
        return NULL;
    }

    return pg;
}

void
pgt_connection_destroy(pgt_connection_t *connection)
{
    config_destroy(&connection->config->conf);

    pgt_free(connection->config->database);
    pgt_free(connection->config);

    PQfinish(connection->conn);
}

static pgt_connection_config_t *
pgt_connection_config_init(u_char *pgt_conf_file)
{
    pgt_connection_config_t *config;
    pgt_connection_config_database_t *database;

    config = pgt_alloc(sizeof(pgt_connection_config_t));
    if (config == NULL) {
        return NULL;
    }

    database = pgt_alloc(sizeof(pgt_connection_config_database_t));
    if (database == NULL) {
        return NULL;
    }

    config_init(&config->conf);
    if (!config_read_file(&config->conf, (const char *)pgt_conf_file)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "%s", config_error_text(&config->conf));
        config_destroy(&config->conf);
        return NULL;
    }

    if (!config_lookup_string(&config->conf, "database.host", &database->host)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"host\" from database in configuration file");
        config_destroy(&config->conf);
        return NULL;
    }

    if (!config_lookup_string(&config->conf, "database.username", &database->username)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"username\" from database in configuration file");
        config_destroy(&config->conf);
        return NULL;
    }

    if (!config_lookup_string(&config->conf, "database.password", &database->password)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"password\" from database in configuration file");
        config_destroy(&config->conf);
        return NULL;
    }

    if (!config_lookup_string(&config->conf, "database.name", &database->name)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"name\" from database in configuration file");
        config_destroy(&config->conf);
        return NULL;
    }

    config->database = database;

    return config;
}