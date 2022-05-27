#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.cpp$//'`
  echo "g++ -std=c++14 -O2 -Wall -Xpreprocessor -fopenmp -lomp -o ${target} ${source}"
  g++ -std=c++14 -O2 -Wall -Xpreprocessor -fopenmp -lomp -o ${target} ${source}
done
