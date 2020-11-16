# vim:set ft=dockerfile:
FROM alpine:3.12

LABEL maintainer="Alan Placidina Maria <placidina@protonmail.com>"
LABEL autor="Alan Placidina Maria <placidina@protonmail.com>"

ENV LANG pt_BR.utf8
ENV TZ=America/Sao_Paulo

RUN set -eux; \
    mkdir /etc/pgtools \
    && mkdir /tmp/pgtools

RUN set -eux; \
    adduser --home /home/pgtools --shell /bin/ash --disabled-password -G users pgtools

RUN set -eux; \
    apk add --no-cache \
    libconfig \
    libpq \
    postgresql-libs \
    tzdata \
    && apk add --no-cache --virtual .build-deps \
    gcc \
    cmake \
    libconfig-dev \
    postgresql-dev \
    make \
    libc-dev \
    linux-headers \
    git

ADD . /tmp/pgtools/

RUN set -eux; \
    cd /tmp/pgtools \
    && mkdir build \
    && cd build/ \
    && cmake ../ \
    && make \
    && make install

RUN set -eux; \
    apk del .build-deps \
    && rm -rf /tmp/*

COPY pgtools.conf.example /etc/pgtools/pgtools.conf

STOPSIGNAL SIGTERM

USER pgtools
CMD [ "pgtools" ]