#include <pgt-config.h>
#include <pgt-core.h>

pgt_modules_t *pgt_modules;

static pgt_modules_config_t *pgt_modules_config_init(u_char *pgt_conf_file);

pgt_int_t
pgt_modules_init(pgt_connection_t *connection, u_char *pgt_conf_file)
{
    pgt_modules_transactions_t *transactions;

    pgt_modules = pgt_alloc(sizeof(pgt_modules_t));
    if (pgt_modules == NULL) {
        return PGT_ERROR;
    }

    transactions = pgt_alloc(sizeof(pgt_modules_transactions_t));
    if (pgt_modules == NULL) {
        return PGT_ERROR;
    }

    pgt_modules->config = pgt_modules_config_init(pgt_conf_file);
    if (pgt_modules->config == NULL) {
        return PGT_ERROR;
    }

    transactions->idle = pgt_modules_transactions_idle_init(connection, pgt_conf_file);
    if (transactions->idle == NULL) {
        return PGT_ERROR;
    }

    pgt_modules->connection = connection;
    pgt_modules->transactions = transactions;

    return PGT_OK;
}

pgt_int_t
pgt_modules_cycle()
{
    while (1) {
        if (pgt_signal_terminate) {
            break;
        }

        if (pgt_modules->transactions->idle->config->enabled) {
            if (pgt_modules_transactions_idle_timeout_kill(pgt_modules->transactions->idle) == PGT_AGAIN) {
                continue;
            }
        }

        if (pgt_signal_terminate) {
            break;
        }

        sleep(pgt_modules->config->cycle);
    }

    return PGT_OK;
}

void
pgt_modules_destroy()
{
    pgt_modules_transactions_idle_destroy(pgt_modules->transactions->idle);

    config_destroy(&pgt_modules->config->conf);

    pgt_free(pgt_modules->transactions->idle);
    pgt_free(pgt_modules->transactions);
    pgt_free(pgt_modules);
}

static pgt_modules_config_t *
pgt_modules_config_init(u_char *pgt_conf_file)
{
    pgt_modules_config_t *config;

    config = pgt_alloc(sizeof(pgt_modules_config_t));
    if (config == NULL) {
        return NULL;
    }

    config_init(&config->conf);
    if (!config_read_file(&config->conf, (const char *)pgt_conf_file)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "%s", config_error_text(&config->conf));
        config_destroy(&config->conf);
        return NULL;
    }

    if (!config_lookup_int(&config->conf, "modules.cycle", &config->cycle)) {
        pgt_log_error(PGT_LOG_EMERG, 0, "Unable to retrieve \"cycle\" for module from configuration file");
        config_destroy(&config->conf);
        return NULL;
    }

    return config;
}