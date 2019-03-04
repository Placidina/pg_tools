/*-------------------------------------------------------------------------
 *
 * pg_log.c
 *		Write logging.
 *
 *
 * IDENTIFICATION
 *		pg_tools/pg_log.c
 *
 *-------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include "pg_log.h"

#ifndef DEBUG
static struct pg_log_t {
    FILE *fp;
    const char *path;
} pg_log_t;

void pg_log_initialize(const char *path) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    pg_log_t.fp = fopen(path, "a+");
    if (pg_log_t.fp == NULL) {
        fprintf(stderr,
                "%d/%d/%d %d:%d:%d error: Could not open log file \"%s\"",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
                tm.tm_min, tm.tm_sec, path);
        exit(EXIT_FAILURE);
    }

    pg_log_t.path = path;
}

void pg_log_reload() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if (!fclose(pg_log_t.fp)) {
        fprintf(stderr,
                "%d/%d/%d %d:%d:%d error: Could not close log file \"%s\"",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
                tm.tm_min, tm.tm_sec, pg_log_t.path);
        exit(EXIT_FAILURE);
    }

    pg_log_initialize(pg_log_t.path);
}

void pg_log_destroy() { fclose(pg_log_t.fp); }
#endif

void pg_log(int level, const char *message, ...) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

#ifndef DEBUG
    if (pg_log_t.fp == NULL) {
        fprintf(stderr, "%d/%d/%d %d:%d:%d error: Uninitialized log file",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
                tm.tm_min, tm.tm_sec);
        exit(EXIT_FAILURE);
    }
#endif

    va_list args;
    va_start(args, message);

    char buffer[LOG_BUFFER];
    vsprintf(buffer, message, args);

    if (level == PG_LOG_ERROR) {
#ifdef DEBUG
        fprintf(stderr, "%d/%d/%d %d:%d:%d error: %s", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                buffer);
#else
        fprintf(pg_log_t.fp, "%d/%d/%d %d:%d:%d error: %s", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                buffer);
        fflush(pg_log_t.fp);
#endif
    } else if (level == PG_LOG_WARNING) {
#ifdef DEBUG
        fprintf(stdout, "%d/%d/%d %d:%d:%d warning: %s", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                buffer);
#else
        fprintf(pg_log_t.fp, "%d/%d/%d %d:%d:%d warning: %s", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                buffer);
        fflush(pg_log_t.fp);
#endif
    } else {
#ifdef DEBUG
        fprintf(stdout, "%d/%d/%d %d:%d:%d %s", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                buffer);
#else
        fprintf(pg_log_t.fp, "%d/%d/%d %d:%d:%d %s", tm.tm_year + 1900,
                tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                buffer);
        fflush(pg_log_t.fp);
#endif
    }

    va_end(args);
}