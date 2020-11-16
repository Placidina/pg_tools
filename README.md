# PG Tools

Simple tool to solve problems for PostgreSQL

## Requeriments

* `postgresql-libs`
* `libconfig`
* `gcc`
* `cmake`
* `make`
* `linux-headers`

## Build

Build default in `container` _(required docker)_.

To build in locally:

```sh
git clone https://github.com/Placidina/pg_tools.git
cd pg_tools
mkdir build/
cd build/
cmake ../
make
```

## Usage Locally

```sh
pgtools version: b66de8cdff390d7883b91113e2d5c957cfb1a72e
Usage: pgt [-?hvudc] 

Options:
  -?,-h               : this help
  -v                  : show version and exit
  -c                  : set configuration file
```

## Available Modules

| Name | Description |
|---|---|
| `transactions/idle` | Kill connections in `idle in transactions` |

### Module Transaction Idle Configurations

| Name | Type | Description |
|---|---|---|
| `enabled` | Bool | Enable or disable `Idle` module |
| `timeout` | Int | Interval entry `NOW() - state_change` to kill transaction |
| `session` | Bool | Enable or disable to kill query `SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;` |

```sh
...
idle = {
	enabled = true;
	timeout = 60;
	session = false;
};
...
```
