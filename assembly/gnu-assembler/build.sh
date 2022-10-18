#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.s$//'`
  echo "as -o ${target}.o ${source}"
  as -o ${target}.o ${source}
  echo "ld -o ${target} ${source}.o"
  ld -o ${target} ${target}.o
done
