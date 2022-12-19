#!/bin/sh

PATH=$PATH:$(dirname $0)

. function/SystemType.sh
. function/GetBool.sh

CMDNAME=$(basename $0)
USAGE="Usage: ${CMDNAME} [-signal] processname"

CURRENT_IFS=$IFS
SIGNAL=
NAME=
PID=
PROCNAME=
OWNER=
PS_OPTS=

PROCESS_LIST=/tmp/list.$$
TITLE_PRENTED=FALSE
FOUND=FALSE

LINE=
COL=

SYSTEM=$(SystemType)

trap 'rm -f /tmp/*.$$; exit 1' 1 2 3 15

case $1 in
  -- )
    shift
    ;;
  -* )
    SIGNAL=$1
    shift
    ;;
esac

if [ $# -ne 1 ]
then
  echo "${USAGE}" 1>&2
  exit 1
fi

NAME=$1

case ${SYSTEM} in
  "Linux"   ) PS_OPTS="auxw"  ;;
  "mac"     ) PS_OPTS="-Af"   ;;
  "FreeBSD" ) PS_OPTS="-auxw" ;;
  "Sun BSD" ) PS_OPTS="-auxw" ;;
  "ULTRIX"  ) PS_OPTS="-auxw" ;;
  *         ) PS_OPTS="-ef"   ;;
esac

ps ${PS_OPTS}  |
sed '1d'       |
grep "${NAME}" |
grep -v "$0"   |
grep -v "ps ${PS_OPTS}" > ${PROCESS_LIST}

exec < ${PROCESS_LIST}

IFS=

while read LINE
do
  IFS=${CURRENT_IFS}

  set ${LINE}
  OWNER=$1
  PID=$2

  case ${SYSTEM} in
    "Linux"   ) COL=64 ;;
    "mac"     ) COL=64 ;;
    "FreeBSD" ) COL=63 ;;
    "AIX"     ) COL=48 ;;
    "HP"      ) COL=48 ;;
    "SGI"     ) COL=48 ;;
    "Solaris" ) COL=48 ;;
    "Sun BSD" ) COL=57 ;;
    "DECOSF"  ) COL=57 ;;
    "ULTRIX"  ) COL=51 ;;
    * )
      echo "Unexpected system type." 1>&2
      exit 1
      ;;
  esac

  LINE=$(echo ${LINE} | cut -c${COL}-)

  set dummy ${LINE}

  shift

  PROCNAME=$1

  # if [ "${PROCNAME}" = "${NAME}" -o "$(basename ${PROCNAME})" = "${NAME}" ]
  if [ "${PROCNAME}" = "${NAME}" ]
  then
    FOUND=TRUE

    if [ "${TITLE_PRENTED}" = "FALSE" ]
    then
      echo "PID Owner Process"
      TITLE_PRENTED=TRUE
    fi

    if GetBool "${PID} ${OWNER} ${PROCNAME} (y/n) ? " < /dev/tty
    then
      kill ${SIGNAL} ${PID}
    fi
  fi

  IFS=
done

if [ "${FOUND}" = "FALSE" ]
then
  echo "Process \"${NAME}\" not found"
fi

rm -f /tmp/*.$$
exit 0
