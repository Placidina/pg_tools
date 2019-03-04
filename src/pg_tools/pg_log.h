#ifndef PG_LOG
#define PG_LOG

#define LOG_BUFFER 4096

typedef enum { PG_LOG_INFO, PG_LOG_WARNING, PG_LOG_ERROR } pg_log_level;

#ifndef DEBUG
void pg_log_initialize(const char *path);
void pg_log_reload();
void pg_log_destroy();
#endif

void pg_log(int level, const char *message, ...);

#endif