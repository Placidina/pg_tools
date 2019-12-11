DOCKER_RUN = docker run --rm -v $(shell pwd):/src/github.com/placidina/pg_tools
CONTAINER = placidina/pg-tools:build
BUILD_IN_CONTAINER = 1

PROJECT=pg-tools
BUILD=RELEASE
VERSION=v1.1.0
DISTDIR=build
OBJDIR=obj
CC=gcc
ARCH=-m64
LDFLAGS=-l pq -l config++
CPPFLAGS=-Wall
CFLAGS=-D version=$(VERSION) -D $(BUILD) -std=c11

SRCS=$(shell find . -name '*.c' -not -path 'build/*' -not -path '.vscode/*' | sed 's/^.\{2\}//')
INC=$(shell find . -name '*.h' -not -path 'build/*' -not -path 'obj/*' -not -path '.vscode/*' -printf '-I %h/\n' | sed 's/\.\///g')

OBJS=$(patsubst %.c,${OBJDIR}/%.o,${SRCS})
DEPS=$(patsubst %.c,${OBJDIR}/%.d,${SRCS})

all: $(DISTDIR)/$(PROJECT)

$(DISTDIR)/$(PROJECT): $(OBJS)
	mkdir -p $(@D)
ifeq ($(BUILD_IN_CONTAINER),1)
	$(DOCKER_RUN) $(CONTAINER) $(CC) $(ARCH) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) $(INC) $^ -o $(DISTDIR)/$(PROJECT)
else
	$(CC) $(ARCH) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) $(INC) $^ -o $(DISTDIR)/$(PROJECT)
endif

.SECONDEXPANSION:

$(OBJS) : ${OBJDIR}/%.o : %.c | $$(dir $$@)
	$(CC) $(CFLAGS) $(INC) -c $< -MD -MP -o $@

${OBJDIR}/%.d : ;

${OBJDIR}/% :
	mkdir -p $@

.PHONY: all clean install uninstall

-include ${DEPS}