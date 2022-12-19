#!/bin/sh

ERROR=0
LINE=

CURRENT_IFS=$IFS

while [ $# -gt 0 ]
do
  if [ ! -r $1 ]
  then
    echo "Cannot find file $1" 1>&2
    ERROR=1
  else
    IFS=

    while read LINE
    do
      echo "${LINE}"
    done < $1
  fi

  shift
done

IFS=${CURRENT_IFS}

exit ${ERROR}
