#!/bin/sh

CMDNAME=$(basename $0)
CWD=$(pwd)
TARGET=

if [ $# -ne 2 ]
then
  echo "Usage: ${CMDNAME} directory1 directory2" 1>&2
  exit 1
fi

if [ ! -d "$1" ]
then
  echo "$1 is not a directory." 1>&2
  exit 1
fi

if [ -f "$2" ]
then
  echo "$2 is already exists." 1>&2
  exit 1
fi

if [ ! -d $2 ]
then
  mkdir -p $2
fi

cd $2
TARGET=$(pwd)
cd ${CWD}

cd $1
find . -depth -print | cpio -pdmu ${TARGET} 2>&1 | grep -iv "blocks"
