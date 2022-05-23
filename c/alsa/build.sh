#!/bin/sh

for source in $@
do
  target=`echo ${source} | sed 's/\.c$//'`
  echo "gcc -std=gnu99 -O2 -Wall -o ${target} ${source} -lm -lasound -lFLAC -lsndfile"
  gcc -std=gnu99 -O2 -Wall -o ${target} ${source} -lm -lasound -lFLAC -lsndfile
done
