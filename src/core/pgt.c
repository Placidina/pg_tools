#include <pgt-config.h>
#include <pgt-core.h>
#include <pgt.h>

static void pgt_show_version_info(void);
static pgt_int_t pgt_get_options(int argc, char *const *argv);

static pgt_uint_t pgt_show_help;
static pgt_uint_t pgt_show_version;
static u_char *pgt_conf_file;

int pgt_cdecl
main(int argc, char *const *argv)
{
    pgt_connection_t *pgt_connection;

    if (pgt_strerror_init() != PGT_OK) {
        return 1;
    }

    if (pgt_get_options(argc, argv) != PGT_OK) {
        return 1;
    }

    if (pgt_show_version || pgt_show_help) {
        pgt_show_version_info();
        return 0;
    }

    pgt_time_init();

    pgt_max_sockets = -1;
    if (pgt_process_init() != PGT_OK) {
        return 1;
    }

    if (pgt_conf_file == NULL) {
        pgt_str_set(pgt_conf_file, PGT_CONF_PATH);
    }

    pgt_connection = pgt_connection_init(pgt_conf_file);
    if (pgt_connection == NULL) {
        return 1;
    }

    if (pgt_modules_init(pgt_connection, pgt_conf_file) == PGT_ERROR) {
        pgt_connection_destroy(pgt_connection);
        return 1;
    }

    pgt_log_error(PGT_LOG_INFO, 0, "pg-tools: %s", pgt_value(SHA1));

    pgt_modules_cycle();

    pgt_modules_destroy();
    pgt_connection_destroy(pgt_connection);

    return 0;
}

static void
pgt_show_version_info(void)
{
    pgt_write_stderr("pgtools version: " pgt_value(SHA1) PGT_LINEFEED);

    if (pgt_show_help) {
        pgt_write_stderr(
            "Usage: pgt [-?hvudc] " PGT_LINEFEED PGT_LINEFEED
            "Options:" PGT_LINEFEED
            "  -?,-h               : this help" PGT_LINEFEED
            "  -v                  : show version and exit" PGT_LINEFEED
            "  -c                  : set configuration file" PGT_LINEFEED);
    }
}

static pgt_int_t
pgt_get_options(int argc, char *const *argv)
{
    u_char *p;
    pgt_int_t i;

    for (i = 1; i < argc; i++) {
        p = (u_char *)argv[i];

        if (*p++ != '-') {
            pgt_log_stderr(0, "invalid option: \"%s\"", argv[i]);
            return PGT_ERROR;
        }

        while (*p) {
            switch (*p++) {
                case '?':
                case 'h':
                    pgt_show_version = 1;
                    pgt_show_help = 1;
                    break;

                case 'v':
                    pgt_show_version = 1;
                    break;

                case 'c':
                    if (*p) {
                        pgt_conf_file = p;
                        goto next;
                    }

                    if (argv[++i]) {
                        pgt_conf_file = (u_char *)argv[i];
                        goto next;
                    }

                    pgt_log_stderr(0, "option \"-c\" requires file name");
                    return PGT_ERROR;
                    break;

                default:
                    pgt_log_stderr(0, "invalid option: \"%c\"", *(p - 1));
                    return PGT_ERROR;
            }
        }

    next:
        continue;
    }

    return PGT_OK;
}