#include <pgt-config.h>
#include <pgt-core.h>

static pgt_modules_transactions_idle_config_t *pgt_modules_transactions_idle_config_init(u_char *pgt_conf_file);

pgt_modules_transactions_idle_t *
pgt_modules_transactions_idle_init(pgt_connection_t *connection, u_char *pgt_conf_file)
{
    pgt_modules_transactions_idle_t *idle;

    idle = pgt_alloc(sizeof(pgt_modules_transactions_idle_t));
    if (idle == NULL) {
        return NULL;
    }

    idle->config = pgt_modules_transactions_idle_config_init(pgt_conf_file);
    if (idle->config == NULL) {
        return NULL;
    }

    idle->conn = connection->conn;

    return idle;
}

pgt_int_t
pgt_modules_transactions_idle_timeout_kill(pgt_modules_transactions_idle_t *idle)
{
    int pid, rows;
    char query[255];
    PGresult *res;
    pgt_flag_t again = 0;

    sprintf(query,
            "SELECT pid, datname, application_name, xact_start, state_change, "
            "query FROM pg_stat_activity WHERE (NOW() - state_change) > "
            "INTERVAL '%d seconds' AND state = 'idle in transaction' ORDER BY "
            "xact_start;",
            idle->config->timeout);

#ifdef PGT_DEBUG
    pgt_log_debug("(modules:transactions:idle) query: %s", query);
#endif

    res = PQexec(idle->conn, query);
    switch (PQresultStatus(res)) {
        case PGRES_FATAL_ERROR:
            pgt_log_error(PGT_LOG_ERR, 0, "%s", PQerrorMessage(idle->conn));

            PQclear(res);
            return PGT_ERROR;

        case PGRES_TUPLES_OK: {
            rows = PQntuples(res);

#ifdef PGT_DEBUG
            pgt_log_debug("(modules:transactions:idle) total: %d", rows);
#endif

            for (int i = 0; i < rows; i++) {
                pid = atoi(PQgetvalue(res, i, 0));

                if (!idle->config->session && pgt_strncmp(MODULES_TRANSACTIONS_IDLE_SESSIN_SKIP, PQgetvalue(res, i, 5), 11) == 0) {
#ifdef PGT_DEBUG
                    pgt_log_debug("(modules:transactions:idle) skiped transaction session pid=\"%d\": %s", pid, PQgetvalue(res, i, 5));
#endif
                    continue;
                }

                pgt_log_error(PGT_LOG_INFO, 0, "pid=\"%d\" datname=\"%s\" application_name=\"%s\" xact_start=\"%s\" state_change=\"%s\": %s", pid, PQgetvalue(res, i, 1), PQgetvalue(res, i, 2), PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5));
                sprintf(query, "SELECT pg_terminate_backend(%d);", pid);

#ifdef PGT_DEBUG
                pgt_log_debug("(modules:transactions:idle) query: %s", query);
#endif

                (void)PQexec(idle->conn, query);

                again = 1;
            }

            PQclear(res);

            if (again == 1) {
#ifdef PGT_DEBUG
                pgt_log_debug("(modules:transactions:idle) AGAIN requested");
#endif
                return PGT_AGAIN;
            }

            return PGT_OK;
        }

        default:
            PQclear(res);
            return PGT_OK;
    }

    return PGT_OK;
}

void
pgt_modules_transactions_idle_destroy(pgt_modules_transactions_idle_t *idle)
{
    config_destroy(&idle->config->conf);
    pgt_free(idle->config);
}

static pgt_modules_transactions_idle_config_t *
pgt_modules_transactions_idle_config_init(u_char *pgt_conf_file)
{
    pgt_modules_transactions_idle_config_t *config;

    config = pgt_alloc(sizeof(pgt_modules_transactions_idle_config_t));
    if (config == NULL) {
        return NULL;
    }

    config_init(&config->conf);
    if (!config_read_file(&config->conf, (const char *)pgt_conf_file)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "%s", config_error_text(&config->conf));
        config_destroy(&config->conf);
        return NULL;
    }

    if (!config_lookup_bool(&config->conf, "modules.transactions.idle.enabled", &config->enabled)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"enable\" for module \"idle\" from configuration file");
        config_destroy(&config->conf);
        return NULL;
    }

    if (config->enabled) {
        if (!config_lookup_int(&config->conf, "modules.transactions.idle.timeout", &config->timeout)) {
            pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"timeout\" for module \"idle\" from configuration file");
            config_destroy(&config->conf);
            return NULL;
        }

        if (!config_lookup_bool(&config->conf, "modules.transactions.idle.session", &config->session)) {
            pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"session\" for module \"idle\" from configuration file");
            config_destroy(&config->conf);
            return NULL;
        }
    }

    return config;
}