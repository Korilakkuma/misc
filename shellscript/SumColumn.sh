#!/bin/sh

CMDNAME=$(basename $0)

if [ $# -gt 1 ]
then
  echo "Usage: ${CMDNAME} [column]" 1>&2
  exit 1
fi

COLUMN=${1:-1}

expr ${COLUMN} + 1 >/dev/null 2>&1

if [ $? -ge 2 ]
then
  echo "Usage: ${CMDNAME} argument is not numeric." 1>&2
  exit 1
fi

awk '{ total += $'${COLUMN}' } END { print total }'
