#!/bin/sh

HOST=${1:-$(hostname)}
HOST=$(echo ${HOST} | sed -e 's/\..*//')

cat /etc/hosts |
sed -e 's/#.*//' -e 's/ / /g' -e 's/  */ /g' -e 's/ *$//g' -e 's/^ *//g' -e "s/ ${HOST}[. ].*/ ${HOST}/g" |
sed -n "/ ${HOST}$/p" |
sed -e 's/ .*//'

exit 0
