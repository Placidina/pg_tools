#
# By Placidina
# https://github.com.br/Placidina/pg_tools
#

PROJECT=pg_tools
BUILD=DEBUG
VERSION=v1.0.3
DISTDIR=dist
OBJDIR=obj
CC=gcc
ARCH=-m64
LDFLAGS=-l pq -l config++
CPPFLAGS=-Wall
CFLAGS=-D version=$(VERSION) -D $(BUILD) -std=c11

SRCS=$(shell find . -name '*.c' -not -path 'dist/*' -not -path '.vscode/*' | sed 's/^.\{2\}//')
INC=$(shell find . -name '*.h' -not -path 'dist/*' -not -path 'obj/*' -not -path '.vscode/*' -printf '-I %h/\n' | sed 's/\.\///g')

SERVICE_SRC := $(shell pidof systemd > /dev/null && echo "resources/lib/systemd/system/pg_tools.service" || echo "resources/etc/init.d/pg_tools")
SERVICE_DEST := $(shell pidof systemd > /dev/null && echo "lib/systemd/system/" || echo "etc/init.d/")
REMOVE_SERVICE := $(shell pidof systemd > /dev/null && echo "systemctl stop pg_tools.service && systemctl disable pg_tools.service && rm -rf /lib/systemd/system/pg_tools.service" || echo "/etc/init.d/pg_tools stop && rm -rf /etc/init.d/pg_tools")

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

install:
	install -m 644 resources/etc/logrotate.d/$(PROJECT) $(DESTDIR)/etc/logrotate.d/
	mkdir -p $(DESTDIR)/etc/$(PROJECT)/
	install -m 644 resources/etc/$(PROJECT)/$(PROJECT).conf $(DESTDIR)/etc/$(PROJECT)/
	install -m 755 $(SERVICE_SRC) $(DESTDIR)/$(SERVICE_DEST)
	install -m 755 dist/$(PROJECT) $(DESTDIR)/usr/bin/
	mkdir -p $(DESTDIR)/var/log/$(PROJECT)/
	touch $(DESTDIR)/var/log/$(PROJECT)/$(PROJECT).log

uninstall:
	rm -rf /etc/logrotate.d/$(PROJECT)
	rm -rf /etc/$(PROJECT)/
	$(REMOVE_SERVICE)
	rm -rf /usr/bin/$(PROJECT)
	rm -rf /var/run/$(PROJECT)/

.PHONY: all clean install uninstall

-include ${DEPS}