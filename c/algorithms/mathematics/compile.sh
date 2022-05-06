#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.c$//'`
  echo "clang -Wall -O2 -std=c99 -o ${target} ${source}"
  clang -Wall -O2 -std=c99 -o ${target} ${source}
done
