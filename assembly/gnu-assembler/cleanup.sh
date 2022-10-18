#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.s$//'`

  if [ -f ${target}.o ]
  then
    echo "rm ${target}.o"
    rm ${target}.o
  fi

  if [ -f ${target} ]
  then
    echo "rm ${target}"
    rm ${target}
  fi
done
