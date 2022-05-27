#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.c$//'`
  echo "gcc -std=c99 -O2 -Wall -Xpreprocessor -fopenmp -lomp -o ${target} ${source}"
  gcc -std=c99 -O2 -Wall -Xpreprocessor -fopenmp -lomp -o ${target} ${source}
done
