#!/bin/sh

if [ $# -eq 1 ]
then
  if [ $1 = "-r" ]
  then
    rm -rf /tmp/bbsd
    exit 0
  fi
fi

if [ ! -d "/tmp/bbsd" ]
then
  mkdir /tmp/bbsd
fi

echo "aaa\naaaa\naaaaa" > /tmp/bbsd/data1
echo "bbb\nbbbb\nbbbbb" > /tmp/bbsd/data2
echo "ccc\ncccc\nddddd" > /tmp/bbsd/data3
echo "ddd\ndddd\nddddd" > /tmp/bbsd/data4
