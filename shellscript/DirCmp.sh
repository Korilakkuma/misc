#!/bin/sh

CMDNAME=$(basename $0)
USAGE="Usage: ${CMDNAME} [-v] [dir1] [dir2]"

CWD=$(pwd)

DIR1=
DIR2=

DIR1_FILES=/tmp/files1.$$
DIR2_FILES=/tmp/files2.$$
ALL_FILES=/tmp/allfiles.$$
COMMON_FILES=/tmp/comfiles.$$

TMP=/tmp/tmp.$$
FOUND=FALSE
FIRST=
VERBOSE=FALSE

trap $(rm -f /tmp/*.$$; exit 1) 1 2 3 15

while :
do
  case $1 in
    -v )
     VERBOSE=TRUE
     shift
     ;;
    -- )
     shift
     break
     ;;
    -* )
      echo "${USAGE}" 1>&2
      exit 1
      ;;
    * )
      break
      ;;
  esac
done

if [ $# -eq 1 ]
then
  DIR1="."
  DIR2="$1"
elif [ $# -eq 2 ]
then
  DIR1="$1"
  DIR2="$2"
else
  echo "${USAGE}" 1>&2
  exit 1
fi

if [ ! -d ${DIR1} ]
then
  echo "${DIR1} is not a directory." 1>&2
  exit 1
fi

if [ ! -d ${DIR2} ]
then
  echo "${DIR2} is not a directory." 1>&2
  exit 1
fi

cd ${DIR1}
find . \( -type f \) -print | sort > ${DIR1_FILES}
# find . \( -type f -o -type 1 \) -print | sort > ${DIR1_FILES}
cd ${CWD}

cd ${DIR2}
find . \( -type f \) -print | sort > ${DIR2_FILES}
# find . \( -type f -o -type 1 \) -print | sort > ${DIR2_FILES}
cd ${CWD}

cat ${DIR1_FILES} ${DIR2_FILES} | sort | uniq    > ${ALL_FILES}
cat ${DIR1_FILES} ${DIR2_FILES} | sort | uniq -d > ${COMMON_FILES}

cat ${DIR1_FILES} ${ALL_FILES} | sort | uniq -u > ${TMP}

if [ -s ${TMP} ]
then
  FOUND=TRUE

  echo ""
  echo "Files missing from ${DIR1}:"

  for f in $(cat ${TMP})
  do
    f=$(expr ${f} : '..\(.*\)')
    echo "   ${f}"
  done
fi

cat ${DIR2_FILES} ${ALL_FILES} | sort | uniq -u > ${TMP}

if [ -s ${TMP} ]
then
  FOUND=TRUE

  echo ""
  echo "Files missing from ${DIR2}:"

  for f in $(cat ${TMP})
  do
    f=$(expr ${f} : '..\(.*\)')
    echo "   ${f}"
  done
fi

FIRST=TRUE

for f in $(cat ${COMMON_FILES})
do
  cmp -s ${DIR1}/${f} ${DIR2}/${f}

  if [ $? -ne 0 ]
  then
    FOUND=TRUE

    f=$(expr ${f} : '..\(.*\)')

    if [ $? -ne 0 ]
    then
      FIRST=FALSE

      echo ""
      echo "Files that are not the same:"
    fi

    if [ "${VERBOSE}" = "TRUE" ]
    then
      echo ""
      echo "Files: ${f}"

      diff ${DIR1}/${f} ${DIR2}/${f}
    else
      echo "   ${f}"
    fi
  fi
done

rm -f /tmp/*.$$

if [ ${FOUND} = TRUE ]
then
  exit 2
else
  echo "The directories are the same."
  exit 0
fi
