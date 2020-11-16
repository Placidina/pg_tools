#ifndef _PGT_LINUX_H_INCLUDED_
#define _PGT_LINUX_H_INCLUDED_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define _FILE_OFFSET_BITS 64

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <sys/resource.h>

#include <time.h>
#include <malloc.h>

#include <libpq-fe.h>
#include <libconfig.h>

#endif /* _PGT_LINUX_H_INCLUDED_ */