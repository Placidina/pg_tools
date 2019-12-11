# vim:set ft=dockerfile:
FROM centos:7

LABEL maintainer="Alan Placidina Maria <alanplacidina@gmail.com>"

RUN yum update -y \
    && yum install -y libconfig postgresql-libs

RUN ln -sf /dev/stdout /var/log/pg-tools.log

COPY pg-tools.conf.example /etc/pg-tools.d/pg-tools.conf
COPY build/pg-tools /usr/bin/pg-tools
COPY docker-entrypoint.sh /docker-entrypoint.sh

ENTRYPOINT [ "/docker-entrypoint.sh" ]
CMD [ "pg-tools" ]