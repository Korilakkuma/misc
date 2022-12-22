#!/bin/sh

CMDNAME=$(basename $0)

if [ $# -ne 1 ]
then
  echo "Usage: ${CMDNAME} command" 1>&2
  exit 1
fi

FOUND=FALSE
COMMAND=$1
CURRENT_IFS=$IFS

case ${COMMAND} in
  */* )
    if [ -x "${COMMAND}" -a ! -d "${COMMAND}" ]
    then
      echo "${COMMAND}"
      FOUND=TRUE
    fi

    ;;
  *   )
    IFS=:

    for dir in $(echo $PATH | sed -e 's/^:/.:/' -e 's/::/:.:/g' -e 's/:$/:./' -e 's/ /:/g')
    do
      if [ -x "${dir}/${COMMAND}" -a ! -d ${dir}/${COMMAND} ]
      then
        echo "${dir}/${COMMAND}"
        FOUND=TRUE
        break
      fi
    done

    ;;
esac

IFS=${CURRENT_IFS}

if [ "${FOUND}" = "FALSE" ]
then
  echo "${COMMAND} not found"
  exit 2
else
  exit 0
fi
