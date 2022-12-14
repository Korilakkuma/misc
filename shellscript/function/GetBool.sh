GetBool() {
  _BOOL=

  if [ $# -eq 0 ]
  then
    echo "Usage: GetBool message" 1>&2
    exit 1
  fi

  while :
  do
    if [ "$(echo -n)" = "-n" ]
    then
      echo "$@\c"
    else
      echo -n "$@"
    fi

    read _BOOL

    case ${_BOOL} in
      [yY] | yes | YES | Yes ) return 0 ;;
      [nN] | no  | NO  | No  ) return 1 ;;
      * ) echo "Enter y or n." ;;
    esac
  done
}
