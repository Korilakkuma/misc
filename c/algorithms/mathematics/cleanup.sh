#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.c$//'`

  echo "rm ${target}"

  if [ -e ${target} ]
  then
    rm ${target}
  fi
done
