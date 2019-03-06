# PG Tools

Simple tool to solve the problem of `idle in transactions` from PostgreSQL 9.4

## Requeriments

* `postgresql-libs`
* `libconfig`
* `gcc`

## Build

```
git clone https://github.com/Placidina/pg_tools.git
cd pg_tools
make BUILD=RELEASE
make install
```

* Uninstall

```
make uninstall
```

## Daemon

* `systemd`
```
systemctl enable pg_tools.service
systemctl start pg_tools.service
```

* `init.d`
```
/etc/init.d/pg_tools start
```

## Arguments

```
PG Tools v1.0.0

Usage:
	-h	Show this.
	-c	Configuration file.
			Default: /etc/pg_tools/pg_tools.conf

Report bug: <alanplacidina@gmail.com>
```