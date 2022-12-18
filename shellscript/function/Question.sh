Question() {
  if [ $# -lt 3 ]
  then
    echo "Usage: Question message default help" 1>&2
    exit 1
  fi

  ANS=

  _DEFAULT=$2
  _QUESTION=
  _HELPMSG=$3

  if [ "${_DEFAULT}" = "" ]
  then
    _QUESTION="$1? "
  else
    _QUESTION="$1 [${_DEFAULT}]? "
  fi

  while :
  do
    if [ "$(echo -n)" = "-n" ]
    then
      echo "${_QUESTION}\c"
    else
      echo -n "${_QUESTION}"
    fi

    read ANS

    case $(echo ${_ANS} | tr [A-Z] [a-z]) in
      "" )
        if [ "${_DEFAULT}" != "" ]
        then
          ANS=${_DEFAULT}
          break
        fi

        ;;
      y | yes )
        ANS=yes
        break
        ;;
      n | no )
        ANS=no
        break
        ;;
      q | quit )
        exit 1
        ;;
      +x | -x )
        set ${ANS}
        ;;
      !* )
        eval $(expr ${ANS} : "!\(.*\)")
        ;;
      "?" )
        if [ "${_HELPMSG}" = "" ]
        then
          echo "No help."
        else
          echo "${_HELPMSG}"
        fi

        echo ""

        ;;
      * )
        break
        ;;
    esac
  done
}
