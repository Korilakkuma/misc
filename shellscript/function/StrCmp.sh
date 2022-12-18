StrCmp() {
  if [ $# -ne 2 ]
  then
    echo "Usage: StrCmp str1 str2" 1>&2
    exit 1
  fi

  if [ "$1" = "$2" ]
  then
    echo "0"
  else
    _TMP=$({ echo "$1"; echo "$2"; } | sort | sed -n '1p')

    if [ "${_TMP}" = "$1" ]
    then
      echo "-1" # asc
    else
      echo "1" # desc
    fi
  fi
}
