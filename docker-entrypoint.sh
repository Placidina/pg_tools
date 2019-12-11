#!/usr/bin/env bash
set -Eeo pipefail

if [ -n "$PG_HOST" ]; then
    sed -i 's/host="127.0.0.1"/host="'$PG_HOST'"/g' /etc/pg-tools.d/pg-tools.conf
fi

if [ -n "$PG_USER" ]; then
    sed -i 's/username="postgres"/username="'$PG_USER'"/g' /etc/pg-tools.d/pg-tools.conf
fi

if [ -n "$PG_PASSWORD" ]; then
    sed -i 's/password=""/password="'$PG_PASSWORD'"/g' /etc/pg-tools.d/pg-tools.conf
fi

if [ -n "$PG_DATABASE" ]; then
    sed -i 's/name="template1"/name="'$PG_DATABASE'"/g' /etc/pg-tools.d/pg-tools.conf
fi

if [ -n "$PG_TIMEOUT" ]; then
    sed -i 's/timeout=10/timeout='$PG_TIMEOUT'/g' /etc/pg-tools.d/pg-tools.conf
fi

if [ -n "$PG_DAEMON" ]; then
    sed -i 's/daemon=30/daemon='$PG_DAEMON'/g' /etc/pg-tools.d/pg-tools.conf
fi

exec "$@"