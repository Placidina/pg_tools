#ifndef _PGT_STRING_H_INCLUDED_
#define _PGT_STRING_H_INCLDUDED_

#include <pgt-config.h>
#include <pgt-core.h>

typedef struct {
    size_t len;
    u_char *data;
} pgt_str_t;

typedef struct {
    unsigned len : 28;

    unsigned valid : 1;
    unsigned no_cacheable : 1;
    unsigned not_found : 1;
    unsigned escape : 1;

    u_char *data;
} pgt_variable_value_t;

#define pgt_string(str)                \
    {                                  \
        sizeof(str) - 1, (u_char *)str \
    }
#define pgt_null_string \
    {                   \
        0, NULL         \
    }

#define pgt_strcmp(s1, s2)     strcmp((const char *)s1, (const char *)s2)
#define pgt_strncmp(s1, s2, n) strncmp((const char *)s1, (const char *)s2, (size_t)n)
#define pgt_strlen(s)          strlen((const char *)s)
#define pgt_str_set(str, text) str = (u_char *)text

#define pgt_strtok(s1, s2) (u_char *)strtok((char *)s1, (const char *)s2)

#define pgt_memzero(buf, n)     (void)memset(buf, 0, n)
#define pgt_memset(buf, c, n)   (void)memset(buf, c, n)
#define pgt_memcpy(dst, src, n) (void)memcpy(dst, src, n)
#define pgt_cpymem(dst, src, n) (((u_char *)memcpy(dst, src, n)) + (n))

u_char *pgt_cdecl pgt_sprintf(u_char *buf, const char *fmt, ...);
u_char *pgt_cdecl pgt_slprintf(u_char *buf, u_char *last, const char *fmt, ...);
u_char *pgt_vslprintf(u_char *buf, u_char *last, const char *fmt, va_list args);
u_char *pgt_strnbinary(u_char *buf, size_t size, size_t *len);

pgt_int_t pgt_atoi(u_char *line, size_t n);

#define pgt_value_helper(n) #n
#define pgt_value(n)        pgt_value_helper(n)

#endif /* _PGT_STRING_H_INCLUDED_ */