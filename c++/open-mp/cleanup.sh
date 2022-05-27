#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.cpp$//'`

  if [ -f ${target} ]
  then
    echo "rm ${target}"
    rm ${target}
  fi
done
