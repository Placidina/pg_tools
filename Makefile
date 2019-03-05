#
# By Placidina
# https://github.com.br/Placidina/pg_tools
#

PROJECT=pg_tools
BUILD=DEBUG
VERSION=v1.0.0-alpha1
DISTDIR=dist
OBJDIR=obj
CC=gcc
ARCH=-m64
LDFLAGS=-l pq -l config++
CPPFLAGS=-Wall
CFLAGS=-D version=$(VERSION) -D $(BUILD)

SRCS=$(shell find . -name '*.c' -not -path 'dist/*' -not -path '.vscode/*' | sed 's/^.\{2\}//')
INC=$(shell find . -name '*.h' -not -path 'dist/*' -not -path 'obj/*' -not -path '.vscode/*' -printf '-I %h/\n' | sed 's/\.\///g')

OBJS=$(patsubst %.c,${OBJDIR}/%.o,${SRCS})
DEPS=$(patsubst %.c,${OBJDIR}/%.d,${SRCS})

all: $(DISTDIR)/$(PROJECT)

$(DISTDIR)/$(PROJECT): $(OBJS)
	mkdir -p $(@D)
	$(CC) $(ARCH) $(CFLAGS) $(LDFLAGS) $(CPPFLAGS) $(INC) $^ -o $(DISTDIR)/$(PROJECT)

.SECONDEXPANSION:

$(OBJS) : ${OBJDIR}/%.o : %.c | $$(dir $$@)
	$(CC) $(CFLAGS) $(INC) -c $< -MD -MP -o $@

${OBJDIR}/%.d : ;

${OBJDIR}/% :
	mkdir -p $@

clean :
	rm -rf ${OBJDIR}
	rm -rf ${DISTDIR}/${PROJECT}

.PHONY: all clean

-include ${DEPS}