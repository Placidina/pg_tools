# PG Tools

Simple tool to solve the problem of `idle in transactions` from PostgreSQL 9.4

## Requeriments

* `postgresql-libs`
* `libconfig`
* `gcc`

## Build

Build default in `container` _(required docker)_.

To build in locally:

```sh
git clone https://github.com/Placidina/pg_tools.git
cd pg_tools
make BUILD_IN_CONTAINER=0
```

## Usage Locally

```sh
PG Tools v1.0.0

Usage:
	-h	Show this.
	-c	Configuration file.
			Default: /etc/pg-tools.d/pg-tools.conf

Report bug: <alanplacidina@gmail.com>
```

## Usage in Container

```sh
docker run --rm --name pg -e PG_HOST=example.com -e PG_PASSWORD=12345 -d placidina/pg-tools:latest
```

## Available Container Environments

| Env | Description |
|---|---|
| `PG_HOST` | Database server host or socket directory |
| `PG_USER` | Database user name (default: "postgres") |
| `PG_PASSWORD` | User password |
| `PG_DATABASE` | Database name to connect (default: "template1") |
| `PG_TIMEOUT` | Seconds to kill connection "state_change" (default: "10" |
| `PG_DAEMON` | Seconds to next execute (default: "30") |
