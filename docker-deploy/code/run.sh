#!/bin/bash
make clean
make
touch /var/log/erss/proxy.log && rm /var/log/erss/proxy.log
./proxy_server &
while true ; do continue ; done
